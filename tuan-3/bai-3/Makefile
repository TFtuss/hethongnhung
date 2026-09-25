TARGET = bai3
BUILD = build
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size

CFLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -Os -g3 \
         -ffunction-sections -fdata-sections -ffreestanding \
         -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft \
          -Tstm32f103c8.ld -nostartfiles -Wl,--gc-sections \
          -Wl,-Map=$(BUILD)/$(TARGET).map

OBJECTS = $(BUILD)/startup.o $(BUILD)/main.o

all: $(BUILD)/$(TARGET).bin $(BUILD)/$(TARGET).hex

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/startup.o: startup.s | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/main.o: src/main.c include/stm32f103_min.h include/app_config.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJECTS) stm32f103c8.ld
	$(CC) $(LDFLAGS) $(OBJECTS) -lgcc -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

flash: $(BUILD)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

clean:
	rm -rf $(BUILD)

.PHONY: all flash clean
