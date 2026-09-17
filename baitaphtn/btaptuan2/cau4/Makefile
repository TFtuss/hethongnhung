TARGET := cau4
BUILD := build
HAL_DIR ?= $(STM32CUBE_F1)

ifeq ($(strip $(HAL_DIR)),)
$(error Set HAL_DIR or STM32CUBE_F1 to the STM32CubeF1 directory)
endif

PREFIX ?= arm-none-eabi-
CC := $(PREFIX)gcc
AS := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

CPUFLAGS := -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
CFLAGS := $(CPUFLAGS) -std=c11 -Wall -Wextra -Os -ffunction-sections -fdata-sections
CFLAGS += -DSTM32F103xB -DUSE_HAL_DRIVER
CFLAGS += -ICore/Inc
CFLAGS += -I$(HAL_DIR)/Drivers/CMSIS/Include
CFLAGS += -I$(HAL_DIR)/Drivers/CMSIS/Device/ST/STM32F1xx/Include
CFLAGS += -I$(HAL_DIR)/Drivers/STM32F1xx_HAL_Driver/Inc

LDFLAGS := $(CPUFLAGS) -TSTM32F103C8TX_FLASH.ld
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map
LDLIBS := --specs=nano.specs --specs=nosys.specs

HAL_SRC := $(HAL_DIR)/Drivers/STM32F1xx_HAL_Driver/Src
CMSIS_SRC := $(HAL_DIR)/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates
SOURCES := Core/Src/main.c \
           Core/Src/system_stm32f1xx.c \
           $(HAL_SRC)/stm32f1xx_hal.c \
           $(HAL_SRC)/stm32f1xx_hal_cortex.c \
           $(HAL_SRC)/stm32f1xx_hal_gpio.c \
           $(HAL_SRC)/stm32f1xx_hal_rcc.c \
           $(HAL_SRC)/stm32f1xx_hal_rcc_ex.c \
           $(HAL_SRC)/stm32f1xx_hal_tim.c \
           $(HAL_SRC)/stm32f1xx_hal_tim_ex.c
OBJECTS := $(patsubst %.c,$(BUILD)/%.o,$(SOURCES))

.PHONY: all clean flash

all: $(BUILD)/$(TARGET).bin

$(BUILD)/$(TARGET).elf: $(OBJECTS) $(BUILD)/startup_stm32f103xb.o STM32F103C8TX_FLASH.ld
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/startup_stm32f103xb.o: startup_stm32f103xb.s
	@mkdir -p $(dir $@)
	$(AS) $(CPUFLAGS) -c $< -o $@

Core/Src/system_stm32f1xx.c:
	@mkdir -p Core/Src
	cp $(CMSIS_SRC)/system_stm32f1xx.c $@

flash: $(BUILD)/$(TARGET).bin
	st-flash write $< 0x08000000

clean:
	rm -rf $(BUILD) Core/Src/system_stm32f1xx.c