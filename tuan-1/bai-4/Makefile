# Reusable Makefile template for STM32 GNU toolchain
# Per-project overrides: you can set variables on the make command line or
# create a project.mk in the project folder and it will be included.

-include project.mk

# Default project settings (override these as needed)
TARGET ?= firmware
BUILD ?= build

# Toolchain
CC ?= arm-none-eabi-gcc
AS ?= arm-none-eabi-gcc
OBJCOPY ?= arm-none-eabi-objcopy
SIZE ?= arm-none-eabi-size
AR ?= arm-none-eabi-ar

# CPU / flags (change MCU or optimization here)
CPUFLAGS ?= -mcpu=cortex-m3 -mthumb
DEFS ?= -DSTM32F103xB
CFLAGS ?= $(CPUFLAGS) -std=c11 -Os -g3 -ffunction-sections -fdata-sections -Wall -Wextra
CFLAGS += $(DEFS)

# Include directories (set INC_DIRS = dir1 dir2 ... in project.mk or via command line)
INC_DIRS ?=
CFLAGS += $(addprefix -I,$(INC_DIRS))

# Linker script (override if your board uses different script)
LD_SCRIPT ?= TSTM32F103C8Tx_FLASH.ld
LDFLAGS ?= $(CPUFLAGS) -T$(LD_SCRIPT) -nostartfiles -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

# Libraries to link (e.g. -Llibs -lfoo)
LIBS ?=

# Source configuration
# Default to src/ for a Keil-like layout: src/ for .c, Inc/ for headers
SRC_DIR ?= src
# Find all .c recursively under SRC_DIR (sorted). This lets you place sources in subfolders.
C_SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -print | sort)
OBJECTS := $(C_SOURCES:%.c=$(BUILD)/%.o) $(BUILD)/startup.o

.PHONY: all clean flash size

all: $(BUILD)/$(TARGET).bin size

$(BUILD):
	mkdir -p $@

# Compile C sources
$(BUILD)/%.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup
$(BUILD)/startup.o: startup.s | $(BUILD)
	$(AS) $(CPUFLAGS) -c $< -o $@

# Link ELF
$(BUILD)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

# Make binary
$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Size
size: $(BUILD)/$(TARGET).elf
	$(SIZE) $<

# Flash using OpenOCD (ST-Link). To select a specific ST-Link by serial:
# make flash STLINK_SERIAL=0123456789
flash: $(BUILD)/$(TARGET).bin
	openocd -c "set CPUTAPID 0x2ba01477" -f interface/stlink.cfg -f target/stm32f1x.cfg \
		-c "reset_config none; adapter speed 50; $(if $(STLINK_SERIAL),hla_serial $(STLINK_SERIAL); )init; halt; program $< 0x08000000 verify; reset run; exit"

clean:
	rm -rf $(BUILD)
