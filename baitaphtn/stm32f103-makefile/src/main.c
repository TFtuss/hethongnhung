#include <stdint.h>

#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018UL)

#define GPIOA_CRL     (*(volatile uint32_t *)0x40010800UL)
#define GPIOA_CRH     (*(volatile uint32_t *)0x40010804UL)
#define GPIOA_IDR     (*(volatile uint32_t *)0x40010808UL)
#define GPIOA_ODR     (*(volatile uint32_t *)0x4001080CUL)

void GPIO_Init(void)
{
    // Bật clock GPIOA
    RCC_APB2ENR |= (1UL << 2);

    // PA0 - PA7: Output Push-Pull, 2 MHz
    GPIOA_CRL = 0x22222222UL;

    // PA8 - PA12, PA15: Input Pull-up; giữ PA13/PA14 cho SWD
    GPIOA_CRH = (GPIOA_CRH & 0x00FF0000UL) | 0x80088888UL;

    // Chọn pull-up cho PA8 - PA15
    GPIOA_ODR |= 0xFF00UL;
}

int main(void)
{
    GPIO_Init();

    while (1)
    {
        // Đọc dữ liệu từ PA8 - PA15
        uint16_t data = (uint16_t)((GPIOA_IDR >> 8) & 0x00FFUL);

        // Đảo dữ liệu
        data = (~data) & 0x00FF;

        // Tắt PA6
        data &= (uint16_t)~(1U << 6);

        // Ghi dữ liệu đã đảo lên PA0 - PA7
        GPIOA_ODR = (GPIOA_ODR & 0xFF00UL) | data;
    }
}