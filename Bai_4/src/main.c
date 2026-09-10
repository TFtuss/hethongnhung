#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018u)
#define GPIOB_CRH   (*(volatile uint32_t *)0x40010C04u)
#define GPIOB_IDR   (*(volatile uint32_t *)0x40010C08u)
#define GPIOB_ODR   (*(volatile uint32_t *)0x40010C0Cu)
#define GPIOC_CRH   (*(volatile uint32_t *)0x40011004u)
#define GPIOC_ODR   (*(volatile uint32_t *)0x4001100Cu)

static void delay(void)
{
    volatile uint32_t count = 80000u;

    while (count-- != 0u) {
    }
}

int main(void)
{
    RCC_APB2ENR |= (1u << 3);  /* GPIOB clock */
    RCC_APB2ENR |= (1u << 4);  /* GPIOC clock */

    /* PB14 = button, configured as input with pull-up. */
    GPIOB_CRH &= ~(0xFu << 24);
    GPIOB_CRH |=  (0x8u << 24);
    GPIOB_ODR |= (1u << 14);

    /* PC13 = LED, configured as output push-pull. */
    GPIOC_CRH &= ~(0xFu << 20);
    GPIOC_CRH |=  (0x3u << 20);

    /* Blue Pill LED on PC13 is active low. */
    GPIOC_ODR |= (1u << 13);

    while (1) {
        if ((GPIOB_IDR & (1u << 14)) == 0u) {
            delay();                 /* debounce press */

            if ((GPIOB_IDR & (1u << 14)) == 0u) {
                while ((GPIOB_IDR & (1u << 14)) == 0u) {
                }

                delay();             /* debounce release */
                GPIOC_ODR ^= (1u << 13);
            }
        }
    }
}