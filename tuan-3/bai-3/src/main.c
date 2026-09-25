#include <stdint.h>
#include "stm32f103_min.h"
#include "app_config.h"

static volatile uint32_t system_ms;
static volatile uint8_t tx_busy;
static uint8_t tx_queue[MESSAGE_QUEUE_LENGTH][MESSAGE_MAX_LENGTH];
static uint8_t tx_lengths[MESSAGE_QUEUE_LENGTH];
static volatile uint8_t tx_head;
static volatile uint8_t tx_tail;
static uint32_t button_value;

#define BUTTON_PIN 1U /* Dùng PA1 làm nút nhấn, active-low. */

/*
 * system_ms là đồng hồ thời gian đơn giản của chương trình.
 * Từ khóa volatile cần thiết vì biến này được thay đổi trong interrupt.
 */
/* Mỗi lần SysTick tràn sẽ gọi hàm này một lần mỗi 1 ms. */
void SysTick_Handler(void)
{
    system_ms++;
}

static void system_clock_init(void)
{
    /*
     * Blue Pill thường dùng thạch anh HSE 8 MHz.
     * FLASH cần 2 wait state trước khi tăng CPU lên 72 MHz.
     */
    FLASH_ACR = 0x00000002UL;

    /* Bit 16 = HSEON: bật thạch anh ngoài. */
    RCC_CR |= (1UL << 16); /* HSEON */
    /* Bit 17 = HSERDY: chờ HSE ổn định trước khi dùng làm clock. */
    while ((RCC_CR & (1UL << 17)) == 0UL) {
    }

    /*
     * PLLSRC = HSE (bit 16), PLLMUL = x9 (7 << 18).
     * SW = PLL (2 ở bit 0..1) để chọn PLL làm system clock.
     * Kết quả: 8 MHz x 9 = 72 MHz.
     */
    RCC_CFGR = (7UL << 18) | (1UL << 16) | (0x2UL);

    /* Bit 24 = PLLON, bit 25 = PLLRDY. */
    RCC_CR |= (1UL << 24); /* PLLON */
    while ((RCC_CR & (1UL << 25)) == 0UL) {
    }

    /* SWS = PLL: chờ phần cứng xác nhận đã chuyển sang PLL. */
    while ((RCC_CFGR & (0x3UL << 2)) != (0x2UL << 2)) {
    }
}

static void clock_and_gpio_init(void)
{
    system_clock_init();

    /* Cấp clock cho GPIOA và USART1 trên bus APB2. */
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    /* DMA1 nằm trên bus AHB, nên không dùng RCC_APB1ENR. */
    RCC_AHBENR |= RCC_AHBENR_DMA1EN;

    /*
     * PA1 dùng 4 bit ở vị trí 4..7 trong CRL.
     * 1000 = input pull-up/pull-down; BSRR set PA1 lên 1 để chọn pull-up.
     */
    GPIOA_CRL &= ~0x000000F0UL;
    GPIOA_CRL |=  0x00000080UL;
    GPIOA_BSRR = (1UL << BUTTON_PIN);

    /*
     * PA9 dùng nibble 0xB: output 50 MHz, alternate-function push-pull.
     * PA10 dùng nibble 0x4: input floating.
     * Vì vậy USART1_TX đi ra PA9 và USART1_RX đi vào PA10.
     */
    GPIOA_CRH &= ~0x00000FF0UL;
    GPIOA_CRH |=  0x000004B0UL;
}

static void systick_init(void)
{
    /* 72 MHz / 72,000 = 1 kHz, tức mỗi interrupt cách nhau 1 ms. */
    SYST_RVR = 72000UL - 1UL;
    SYST_CVR = 0UL;
    /* CLKSOURCE = CPU clock, TICKINT = bật interrupt, ENABLE = chạy. */
    SYST_CSR = (1UL << 2) | (1UL << 1) | (1UL << 0);
}

static void usart_dma_init(void)
{
    /*
     * USART1 chạy 115200 baud, 8 bit, không parity, 1 stop bit.
     * Với PCLK2 = 72 MHz, BRR = 72,000,000 / 115,200 = 625.
     */
    USART1_BRR = 625UL; /* 72 MHz / 115200 baud. */
    USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
    USART1_CR3 = USART_CR3_DMAT;

    /* DMA lấy dữ liệu từ RAM và ghi từng byte vào thanh ghi USART1_DR. */
    DMA1_CPAR4 = (uint32_t)&USART1_DR;
    /* DIR = RAM -> peripheral, MINC = tăng địa chỉ RAM sau mỗi byte. */
    DMA1_CCR4 = DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_TCIE;
    /* DMA1 Channel 4 dùng cho USART1_TX; bật ngắt khi truyền xong. */
    NVIC_ISER0 = (1UL << 14);
}

static uint8_t format_message(uint8_t *message, uint32_t value)
{
    /*
    * Tạo chuỗi dạng: CLASS_ID + GROUP_ID + ":BTN:" + số + "\n\r".
    * Ví dụ theo cấu hình hiện tại: D23N7:BTN:12\n\r.
     * Phần số được tạo ngược rồi đảo lại vì phép chia lấy chữ số cuối trước.
     */
    static const char digits[] = "0123456789";
    char reversed[11];
    uint8_t position = 0U;
    uint8_t digit_count = 0U;

    for (uint8_t i = 0U; CLASS_ID[i] != '\0'; i++) {
        message[position++] = (uint8_t)CLASS_ID[i];
    }
    for (uint8_t i = 0U; GROUP_ID[i] != '\0'; i++) {
        message[position++] = (uint8_t)GROUP_ID[i];
    }
    message[position++] = ':';
    message[position++] = 'B';
    message[position++] = 'T';
    message[position++] = 'N';
    message[position++] = ':';

    do {
        reversed[digit_count++] = digits[value % 10U];
        value /= 10U;
    } while (value != 0U);
    while (digit_count > 0U) {
        message[position++] = (uint8_t)reversed[--digit_count];
    }
    message[position++] = '\n';
    message[position++] = '\r';
    return position;
}

static void dma_start_next(void)
{
    /*
     * tx_tail trỏ message đang chờ gửi, tx_head trỏ vị trí ghi tiếp theo.
     * Nếu hai vị trí bằng nhau thì queue rỗng.
     */
    if (tx_busy != 0U || tx_tail == tx_head) {
        return;
    }

    tx_busy = 1U;
    /* Phải tắt DMA trước khi đổi địa chỉ và số byte cần truyền. */
    DMA1_CCR4 &= ~DMA_CCR_EN;
    DMA1_CMAR4 = (uint32_t)tx_queue[tx_tail];
    DMA1_CNDTR4 = tx_lengths[tx_tail];
    /* Xóa cờ truyền xong còn sót từ lần truyền trước. */
    DMA1_IFCR = DMA1_IFCR_CTCIF4;
    /* Bật DMA: USART sẽ tự lấy từng byte trong RAM để phát ra PA9. */
    DMA1_CCR4 |= DMA_CCR_EN;
}

static void queue_message(uint32_t value)
{
    /*
     * Queue vòng giúp lưu nhiều message trong RAM.
     * Chừa lại một ô trống để phân biệt queue đầy và queue rỗng.
     */
    uint8_t next_head = (uint8_t)((tx_head + 1U) % MESSAGE_QUEUE_LENGTH);
    if (next_head == tx_tail) {
        return;
    }

    tx_lengths[tx_head] = format_message(tx_queue[tx_head], value);
    tx_head = next_head;
    dma_start_next();
}

void DMA1_Channel4_IRQHandler(void)
{
    /*
     * DMA báo interrupt sau khi đã chuyển đủ số byte.
     * Đánh dấu message hiện tại đã gửi xong, rồi khởi động message kế tiếp.
     */
    if ((DMA1_ISR & DMA1_ISR_TCIF4) != 0U) {
        DMA1_IFCR = DMA1_IFCR_CTCIF4;
        DMA1_CCR4 &= ~DMA_CCR_EN;
        tx_tail = (uint8_t)((tx_tail + 1U) % MESSAGE_QUEUE_LENGTH);
        tx_busy = 0U;
        dma_start_next();
    }
}

int main(void)
{
    uint8_t previous_pressed = 0U;
    uint32_t last_transition_ms = 0U;

    /* Khởi tạo phần cứng trước, sau đó mới bắt đầu vòng lặp chính. */
    clock_and_gpio_init();
    systick_init();
    usart_dma_init();

    while (1) {
        /* Nút active-low: pressed = 1 khi PA1 đang ở mức 0. */
        uint8_t pressed = (GPIOA_IDR & (1UL << BUTTON_PIN)) == 0U;
        if (pressed != previous_pressed && (system_ms - last_transition_ms) >= BUTTON_DEBOUNCE_MS) {
            /* Chỉ chấp nhận thay đổi sau 30 ms để chống dội phím. */
            previous_pressed = pressed;
            last_transition_ms = system_ms;
            if (pressed != 0U) {
                /* Chỉ tăng số một lần ở cạnh nhấn, không tăng khi giữ nút. */
                button_value++;
                queue_message(button_value);
            }
        }
        dma_start_next();
    }
}
