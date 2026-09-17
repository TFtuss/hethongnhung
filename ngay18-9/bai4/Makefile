PROJECT := pwm_4led
BUILD := build
HAL_ROOT ?= ../STM32CubeF1
DEVICE := STM32F103xB
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

CFLAGS := -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -O2 -g3 -Wall -Wextra \
 -ffunction-sections -fdata-sections -ffreestanding -D$(DEVICE) -DUSE_HAL_DRIVER \
 -ICore/Inc -I$(HAL_ROOT)/Drivers/CMSIS/Include \
 -I$(HAL_ROOT)/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
 -I$(HAL_ROOT)/Drivers/STM32F1xx_HAL_Driver/Inc
LDFLAGS := -TSTM32F103C8TX_FLASH.ld -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs
HAL := $(HAL_ROOT)/Drivers/STM32F1xx_HAL_Driver
CMSIS := $(HAL_ROOT)/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates
SRC := Core/Src/main.c $(HAL)/Src/stm32f1xx_hal.c $(HAL)/Src/stm32f1xx_hal_cortex.c \
 $(HAL)/Src/stm32f1xx_hal_dma.c $(HAL)/Src/stm32f1xx_hal_flash.c \
 $(HAL)/Src/stm32f1xx_hal_gpio.c $(HAL)/Src/stm32f1xx_hal_rcc.c \
 $(HAL)/Src/stm32f1xx_hal_tim.c $(HAL)/Src/stm32f1xx_hal_tim_ex.c \
 $(CMSIS)/system_stm32f1xx.c
OBJ := $(patsubst %.c,$(BUILD)/%.o,$(SRC)) $(BUILD)/startup_stm32f103xb.o

all: $(BUILD)/$(PROJECT).hex $(BUILD)/$(PROJECT).bin
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/startup_stm32f103xb.o: startup_stm32f103xb.s
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/$(PROJECT).elf: $(OBJ) STM32F103C8TX_FLASH.ld
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
$(BUILD)/$(PROJECT).hex: $(BUILD)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@
$(BUILD)/$(PROJECT).bin: $(BUILD)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
flash: $(BUILD)/$(PROJECT).bin
	st-flash write $< 0x08000000
clean:
	rm -rf $(BUILD)
.PHONY: all flash clean
