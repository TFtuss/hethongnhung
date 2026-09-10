PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -Wall -Wextra \
         -ffreestanding -fno-builtin

LDFLAGS = -T stm32f103.ld -nostdlib \
          -Wl,--gc-sections -Wl,-Map=firmware.map

all: firmware.bin

firmware.elf: startup.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

startup.o: startup.s
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin *.map
