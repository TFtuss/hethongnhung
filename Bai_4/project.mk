# project.mk — per-project overrides for the top-level Makefile
# Edit these values for each project (or pass on the make command line)

# example target name
TARGET := myproject

# Force ARM cross-toolchain by default to avoid using host cc/as
CC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc

# source directory (relative to repo root)
SRC_DIR := src

# include directories (space-separated)
INC_DIRS := Inc

# linker script (update if using a different board)
LD_SCRIPT := TSTM32F103C8Tx_FLASH.ld

# additional libraries to link (example)
# LIBS := -Llibs -lfoo

# optional: ST-Link serial for targeting a specific probe
# STLINK_SERIAL := 067EFF123456
