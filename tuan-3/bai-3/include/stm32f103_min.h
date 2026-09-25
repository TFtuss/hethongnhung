#ifndef STM32F103_MIN_H
#define STM32F103_MIN_H

#include <stdint.h>

#define REG32(address) (*(volatile uint32_t *)(address))

#define RCC_BASE       0x40021000UL
#define FLASH_BASE     0x40022000UL
#define GPIOA_BASE     0x40010800UL
#define GPIOC_BASE     0x40011000UL
#define USART1_BASE    0x40013800UL
#define DMA1_BASE      0x40020000UL
#define NVIC_ISER0     REG32(0xE000E100UL)

#define RCC_CR         REG32(RCC_BASE + 0x00UL)
#define RCC_CFGR       REG32(RCC_BASE + 0x04UL)
#define RCC_AHBENR     REG32(RCC_BASE + 0x14UL)
#define RCC_APB2ENR    REG32(RCC_BASE + 0x18UL)
#define RCC_APB1ENR    REG32(RCC_BASE + 0x1CUL)
#define FLASH_ACR      REG32(FLASH_BASE + 0x00UL)
#define GPIOA_CRL      REG32(GPIOA_BASE + 0x00UL)
#define GPIOA_CRH      REG32(GPIOA_BASE + 0x04UL)
#define GPIOA_IDR      REG32(GPIOA_BASE + 0x08UL)
#define GPIOA_BSRR     REG32(GPIOA_BASE + 0x10UL)
#define GPIOA_BRR      REG32(GPIOA_BASE + 0x14UL)
#define GPIOC_CRL      REG32(GPIOC_BASE + 0x00UL)
#define GPIOC_CRH      REG32(GPIOC_BASE + 0x04UL)
#define GPIOC_IDR      REG32(GPIOC_BASE + 0x08UL)
#define GPIOC_BSRR     REG32(GPIOC_BASE + 0x10UL)
#define GPIOC_BRR      REG32(GPIOC_BASE + 0x14UL)

#define USART1_SR      REG32(USART1_BASE + 0x00UL)
#define USART1_DR      REG32(USART1_BASE + 0x04UL)
#define USART1_BRR     REG32(USART1_BASE + 0x08UL)
#define USART1_CR1     REG32(USART1_BASE + 0x0CUL)
#define USART1_CR2     REG32(USART1_BASE + 0x10UL)
#define USART1_CR3     REG32(USART1_BASE + 0x14UL)

#define DMA1_ISR       REG32(DMA1_BASE + 0x00UL)
#define DMA1_IFCR      REG32(DMA1_BASE + 0x04UL)
#define DMA1_CCR4      REG32(DMA1_BASE + 0x2CUL)
#define DMA1_CNDTR4    REG32(DMA1_BASE + 0x30UL)
#define DMA1_CPAR4     REG32(DMA1_BASE + 0x34UL)
#define DMA1_CMAR4     REG32(DMA1_BASE + 0x38UL)

#define SYST_CSR       REG32(0xE000E010UL)
#define SYST_RVR       REG32(0xE000E014UL)
#define SYST_CVR       REG32(0xE000E018UL)

#define RCC_APB2ENR_IOPAEN  (1UL << 2)
#define RCC_APB2ENR_IOPCEN  (1UL << 4)
#define RCC_APB2ENR_USART1EN (1UL << 14)
#define RCC_AHBENR_DMA1EN   (1UL << 0)

#define USART_SR_TC          (1UL << 6)
#define USART_CR1_UE         (1UL << 13)
#define USART_CR1_TE         (1UL << 3)
#define USART_CR1_RE         (1UL << 2)
#define USART_CR3_DMAT       (1UL << 7)

#define DMA_CCR_EN           (1UL << 0)
#define DMA_CCR_TCIE         (1UL << 1)
#define DMA_CCR_DIR          (1UL << 4)
#define DMA_CCR_MINC         (1UL << 7)
#define DMA_CCR_PSIZE_8BIT   0UL
#define DMA_CCR_MSIZE_8BIT   0UL
#define DMA1_ISR_TCIF4       (1UL << 13)
#define DMA1_IFCR_CTCIF4     (1UL << 13)

#endif
