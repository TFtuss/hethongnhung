TARGET := bai2
BUILD := build

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

CFLAGS := -mcpu=cortex-m3 -mthumb -O2 -ffreestanding -fno-builtin \
           -fno-stack-protector -Wall -Wextra -Werror
LDFLAGS := -T linker.ld -nostdlib -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

OBJECTS := $(BUILD)/startup.o $(BUILD)/main.o

.PHONY: all clean flash erase info

all: $(BUILD)/$(TARGET).bin

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/startup.o: startup.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/main.o: main.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJECTS) linker.ld
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

erase:
	st-flash erase

info:
	st-info --probe

clean:
	rm -rf $(BUILD)