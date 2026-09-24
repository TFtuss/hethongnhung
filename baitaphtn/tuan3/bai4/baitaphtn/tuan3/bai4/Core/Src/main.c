/*
 * STM32F103C8T6, thạch anh ngoài 8 MHz.
 * TIM3 kích ADC1 đọc cảm biến nhiệt độ nội bộ với tần số 100 Hz.
 * DMA lưu vòng 100 mẫu; mỗi ngắt nửa/đầy buffer gửi 50 mẫu qua USART1.
 * Dữ liệu: giá trị ADC thô, phân cách bằng "\n\r".
 */
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdio.h>

/* 100 mẫu tương ứng 1 giây; chia thành hai nửa, mỗi nửa 50 mẫu. */
#define ADC_BUFFER_SIZE  100U
#define ADC_HALF_SIZE    (ADC_BUFFER_SIZE / 2U)
/* Mỗi mẫu tối đa 4 chữ số + 2 ký tự xuống dòng; thêm 1 byte cho '\0'. */
#define UART_BUFFER_SIZE (ADC_HALF_SIZE * 6U + 1U)

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_usart1_tx;

/* Tách buffer thu ADC và buffer truyền UART để không ghi đè dữ liệu đang gửi. */
volatile uint16_t adc_buffer[ADC_BUFFER_SIZE] __attribute__((aligned(4)));
static uint8_t uart_tx_buffer[UART_BUFFER_SIZE];
static volatile uint8_t uart_tx_busy = 0U;  /* 1: UART đang truyền. */

/* Bộ đếm dùng kiểm tra số lần ngắt và số lô dữ liệu đã gửi. */
volatile uint32_t ht_count = 0U;
volatile uint32_t tc_count = 0U;
volatile uint32_t tx_count = 0U;
/* Mã lỗi: 0 = không lỗi, 1 = UART bận, 2 = định dạng chuỗi,
 * 3 = khởi động truyền, 4 = ADC, 5 = UART. */
volatile uint32_t error_code = 0U;

static void SystemClock_Config(void);
static void DMA_Init(void);
static void ADC1_Init(void);
static void TIM3_Init(void);
static void USART1_Init(void);
static void ADC_SendHalf(uint32_t start_index);
static void Error_Handler(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    DMA_Init();
    ADC1_Init();
    TIM3_Init();
    USART1_Init();

    /* Thông báo kiểm tra khởi động, không phải dữ liệu ADC. */
    static const uint8_t startup_message[] = "STM32 STARTED\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)startup_message,
                      sizeof(startup_message) - 1U, HAL_MAX_DELAY);

    /* Hiệu chuẩn ADC trước khi đo, sau đó chờ cảm biến ổn định. */
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
        Error_Handler();
    HAL_Delay(1U);

    /* Cho ADC-DMA sẵn sàng nhận 100 mẫu; HAL bật cả ngắt HT và TC. */
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer,
                         ADC_BUFFER_SIZE) != HAL_OK)
        Error_Handler();

    /* Khởi động TIM3 sau ADC-DMA để không bỏ lỡ trigger đầu tiên. */
    TIM3->CNT = 0U;
    TIM3->SR = 0U;
    TIM3->CR1 |= TIM_CR1_CEN;

    while (1)
    {
        if (error_code != 0U)
        {
            /* Dừng lấy mẫu khi quá trình thu hoặc truyền gặp lỗi. */
            TIM3->CR1 &= ~TIM_CR1_CEN;
            Error_Handler();
        }
        __WFI();  /* Chờ ngắt, các ngoại vi vẫn hoạt động. */
    }
}

/* Clock hệ thống: HSE 8 MHz x 9 = 72 MHz. */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    RCC_PeriphCLKInitTypeDef peripheral_clk = {0};

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState = RCC_HSE_ON;
    osc.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK)
        Error_Handler();

    clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV2;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2) != HAL_OK)
        Error_Handler();

    /* Clock ADC = PCLK2 / 6 = 12 MHz. */
    peripheral_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    peripheral_clk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&peripheral_clk) != HAL_OK)
        Error_Handler();
}

static void DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* ADC -> RAM: mỗi mẫu 16 bit, tự tăng địa chỉ RAM và ghi vòng liên tục. */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        Error_Handler();

    /* RAM -> UART: truyền từng byte, kết thúc sau mỗi lô dữ liệu. */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
        Error_Handler();

    /* Ưu tiên ngắt DMA thu ADC cao hơn ngắt DMA truyền UART. */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1U, 0U);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 2U, 0U);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

static void ADC1_Init(void)
{
    ADC_ChannelConfTypeDef channel = {0};
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* Mỗi trigger TIM3 chỉ chuyển đổi một mẫu, không chạy liên tục. */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1U;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
        Error_Handler();

    /* Đọc kênh nhiệt độ nội bộ; dùng thời gian lấy mẫu 239,5 chu kỳ ADC. */
    channel.Channel = ADC_CHANNEL_TEMPSENSOR;
    channel.Rank = ADC_REGULAR_RANK_1;
    channel.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &channel) != HAL_OK)
        Error_Handler();

    /* Liên kết ADC1 với DMA1 Channel1. */
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
}

static void TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* Tần số cập nhật = 72 MHz / (7199 + 1) / (99 + 1) = 100 Hz. */
    TIM3->CR1 = 0U;
    TIM3->PSC = 7199U;
    TIM3->ARR = 99U;
    TIM3->CNT = 0U;
    /* MMS = 010: chọn sự kiện cập nhật làm TRGO để kích ADC. */
    TIM3->CR2 = (TIM3->CR2 & ~TIM_CR2_MMS) | (2U << TIM_CR2_MMS_Pos);
    /* Tạo sự kiện cập nhật để nạp prescaler, rồi xóa cờ trạng thái. */
    TIM3->EGR = TIM_EGR_UG;
    TIM3->SR = 0U;
}

static void USART1_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    /* PA9: ngõ ra USART1_TX, nối với RXD của USB-UART. */
    gpio.Pin = GPIO_PIN_9;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);

    /* USART1 chỉ truyền: 115200 baud, 8 bit dữ liệu, không parity, 1 stop bit. */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200U;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
        Error_Handler();

    /* Liên kết bộ truyền USART1 với DMA1 Channel4. */
    __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);

    /* Ngắt USART xác nhận byte cuối đã phát xong sau khi DMA hoàn tất. */
    HAL_NVIC_SetPriority(USART1_IRQn, 2U, 0U);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* Chuyển 50 mẫu của nửa buffer vừa hoàn tất thành chuỗi rồi gửi bằng DMA. */
static void ADC_SendHalf(uint32_t start_index)
{
    if (error_code != 0U)
        return;

    /* Không ghi lại buffer truyền khi UART chưa gửi xong lô trước. */
    if (uart_tx_busy != 0U)
    {
        error_code = 1U;
        return;
    }

    uint32_t length = 0U;
    for (uint32_t i = 0U; i < ADC_HALF_SIZE; ++i)
    {
        unsigned int sample = adc_buffer[start_index + i];
        uint32_t remaining = sizeof(uart_tx_buffer) - length;
        /* Mỗi mẫu là số thập phân, tiếp theo là \n rồi \r. */
        int n = snprintf((char *)&uart_tx_buffer[length], remaining,
                         "%u\n\r", sample);
        if ((n < 0) || ((uint32_t)n >= remaining))
        {
            error_code = 2U;
            return;
        }
        length += (uint32_t)n;
    }

    /* Chỉ gửi các ký tự dữ liệu, không gửi ký tự kết thúc chuỗi \0. */
    uart_tx_busy = 1U;
    if (HAL_UART_Transmit_DMA(&huart1, uart_tx_buffer,
                              (uint16_t)length) != HAL_OK)
    {
        uart_tx_busy = 0U;
        error_code = 3U;
    }
}

/* Half-transfer: nửa đầu đã đủ 50 mẫu, DMA đang ghi nửa sau. */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        ++ht_count;
        ADC_SendHalf(0U);  /* Gửi các mẫu 0..49. */
    }
}

/* Transfer-complete: nửa sau đã đủ 50 mẫu, DMA quay lại ghi nửa đầu. */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        ++tc_count;
        ADC_SendHalf(ADC_HALF_SIZE);  /* Gửi các mẫu 50..99. */
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        ++tx_count;
        uart_tx_busy = 0U;  /* Cho phép sử dụng lại buffer truyền. */
    }
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    if ((hadc->Instance == ADC1) && (error_code == 0U))
        error_code = 4U;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if ((huart->Instance == USART1) && (error_code == 0U))
        error_code = 5U;
}

/* Chuyển xử lý ngắt cho HAL để xóa cờ và gọi callback tương ứng. */
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}

void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

static void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        /* Giữ trạng thái dừng để kiểm tra lỗi bằng debugger. */
    }
}