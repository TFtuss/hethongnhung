# Cau 4 - PWM 4 kenh tren STM32F103

Chuong trinh dung HAL va TIM3 o mapping mac dinh:

| Kenh | Chan | Duty |
| --- | --- | --- |
| TIM3_CH1 | PA6 | 10% |
| TIM3_CH2 | PA7 | 30% |
| TIM3_CH3 | PB0 | 50% |
| TIM3_CH4 | PB1 | 70% |

## Thong so PWM

Voi HSE = 8 MHz, PLL x9 cho SYSCLK = 72 MHz. APB1 = 36 MHz nhung timer tren APB1 duoc nhan 2, do do clock TIM3 = 72 MHz.

```text
f_pwm = 72 MHz / ((71 + 1) * (999 + 1)) = 1 kHz
CCR1 = 100, CCR2 = 300, CCR3 = 500, CCR4 = 700
```

Noi moi LED qua dien tro han dong vao cac chan PA6, PA7, PB0, PB1. Ma nay tao muc cao trong thoi gian duty; neu mach LED active-low, can dao muc logic hoac dung PWM2.

## Build va nap tren Linux

Dat thu muc STM32CubeF1 vao bien moi truong `STM32CUBE_F1`, sau do copy `Core/Src/main.c` va `Core/Inc/main.h` vao project HAL da co startup, linker script va `stm32f1xx_hal_conf.h`.

Vi du voi project CubeIDE/CMake da tao san:

```sh
arm-none-eabi-gcc --version
make
st-flash write build/cau4.bin 0x08000000
```

Can dung dung MCU trong dong STM32F103 khi tao project (vi du STM32F103C8Tx); startup va linker script phai khop voi chip thuc te.