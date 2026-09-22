#define RCC_APB2ENR (*(volatile unsigned long *)0x40021018UL)
#define GPIOA_CRL   (*(volatile unsigned long *)0x40010800UL)
#define GPIOA_ODR   (*(volatile unsigned long *)0x4001080CUL)

static void delay(void)
{
    volatile unsigned long count;

    for (count = 0UL; count < 700000UL; ++count) {
        __asm volatile ("nop");
    }
}

int main(void)
{
    unsigned long led = 0x01UL;
    unsigned long moving_right = 1UL;

    RCC_APB2ENR |= (1UL << 2);
    GPIOA_CRL = 0x22222222UL;

    for (;;) {
        GPIOA_ODR = (GPIOA_ODR & ~0xFFUL) | led;
        delay();

        if (moving_right != 0UL) {
            if (led == 0x80UL) {
                moving_right = 0UL;
                led >>= 1;
            } else {
                led <<= 1;
            }
        } else if (led == 0x01UL) {
            moving_right = 1UL;
            led <<= 1;
        } else {
            led >>= 1;
        }
    }
}