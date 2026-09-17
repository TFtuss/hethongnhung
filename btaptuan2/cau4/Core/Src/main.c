#include "main.h"

TIM_HandleTypeDef htim3;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM3_Init();

    /* TIM3 CH1..CH4 generate 10%, 30%, 50% and 70% duty cycles. */
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK ||
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK ||
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK ||
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK) {
        Error_Handler();
    }

    while (1) {
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc_config = {0};
    RCC_ClkInitTypeDef clock_config = {0};

    osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_config.HSEState = RCC_HSE_ON;
    osc_config.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    osc_config.HSIState = RCC_HSI_ON;
    osc_config.PLL.PLLState = RCC_PLL_ON;
    osc_config.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_config.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&osc_config) != HAL_OK) {
        Error_Handler();
    }

    clock_config.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                             RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clock_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clock_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clock_config.APB1CLKDivider = RCC_HCLK_DIV2;
    clock_config.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clock_config, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_TIM3_Init(void)
{
    TIM_OC_InitTypeDef pwm_config = {0};

    __HAL_RCC_TIM3_CLK_ENABLE();
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 999;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    pwm_config.OCMode = TIM_OCMODE_PWM1;
    pwm_config.Pulse = 100;
    pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    pwm_config.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &pwm_config, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }

    pwm_config.Pulse = 300;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &pwm_config, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }

    pwm_config.Pulse = 500;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &pwm_config, TIM_CHANNEL_3) != HAL_OK) {
        Error_Handler();
    }

    pwm_config.Pulse = 700;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &pwm_config, TIM_CHANNEL_4) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_config = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* TIM3 default pin mapping: PA6, PA7, PB0 and PB1. */
    gpio_config.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio_config);

    gpio_config.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &gpio_config);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}