# STM32F103 PWM 4 LED

Project HAL cho STM32F103C8T6, build tren Linux, khong dung KeilC.

- TIM2_CH1 PA0: 10%
- TIM2_CH2 PA1: 30%
- TIM2_CH3 PA2: 50%
- TIM2_CH4 PA3: 70%
- Tan so PWM: 1 kHz

## Cai dat va build

```bash
sudo apt install gcc-arm-none-eabi stlink-tools
cd /home/phuc
[ -d STM32CubeF1 ] || git clone --depth 1 https://github.com/STMicroelectronics/STM32CubeF1.git
cd STM32CubeF1
git submodule update --init Drivers/CMSIS/Device/ST/STM32F1xx Drivers/STM32F1xx_HAL_Driver
cd /home/phuc/bai4
make
```

## Nap

Ket noi ST-Link, sau do:

```bash
make flash
```

PWM: `72 MHz / (71 + 1) / (999 + 1) = 1 kHz`; CCR 100, 300, 500, 700 tao duty 10%, 30%, 50%, 70%.
