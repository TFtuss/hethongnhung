#define RCC_APB2ENR   (*(volatile unsigned int *)0x40021018U)
#define GPIOC_CRH     (*(volatile unsigned int *)0x40011004U)
#define GPIOC_ODR     (*(volatile unsigned int *)0x4001100CU)

#define GPIOC_CLOCK_ENABLE  (1U << 4)
#define LED_PIN             (1U << 13)

#define BLINK_PERIOD_MS     1000U

static void delay_ms(unsigned int milliseconds)
{
    while (milliseconds-- > 0U) {
        for (volatile unsigned int count = 0U; count < 2667U; count++) {
            __asm volatile("nop");
        }
    }
}

int main(void)
{
    RCC_APB2ENR |= GPIOC_CLOCK_ENABLE;

    // PC13: output push-pull, 10 MHz
    GPIOC_CRH &= ~(0xFU << 20);
    GPIOC_CRH |= (0x1U << 20);

    // Blue Pill LED is active-low, so PC13 = 1 means off.
    GPIOC_ODR |= LED_PIN;

    while (1) {
        GPIOC_ODR ^= LED_PIN;
        delay_ms(BLINK_PERIOD_MS);
    }
}
