.RECIPEPREFIX := >
PREFIX ?= arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size
HOSTCC ?= cc
CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
CFLAGS = $(CPU) -std=gnu11 -Os -g3 -Wall -Wextra -Werror \
         -ffreestanding -fno-builtin -fno-common \
         -ffunction-sections -fdata-sections -MMD -MP \
         -DSTM32F446xx -Iinclude -Ivendor/ST/Include -Ivendor/CMSIS/Include
LDFLAGS = $(CPU) -nostdlib -nostartfiles -TSTM32F446RE.ld \
          -Wl,--gc-sections,--build-id=none,-Map=build/firmware.map
SOURCES = $(wildcard src/*.c)
TESTS = $(wildcard tests/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES)) \
          $(patsubst tests/%.c,build/tests_%.o,$(TESTS))
.PHONY: all clean flash check check-host
all: build/firmware.elf build/firmware.bin build/firmware.hex
>$(SIZE) build/firmware.elf
build:
>mkdir -p build
build/%.o: src/%.c Makefile | build
>$(CC) $(CFLAGS) -c $< -o $@
build/tests_%.o: tests/%.c Makefile | build
>$(CC) $(CFLAGS) -c $< -o $@
build/firmware.elf: $(OBJECTS) STM32F446RE.ld Makefile
>$(CC) $(LDFLAGS) $(OBJECTS) -lgcc -o $@
build/firmware.bin: build/firmware.elf
>$(OBJCOPY) -O binary $< $@
build/firmware.hex: build/firmware.elf
>$(OBJCOPY) -O ihex $< $@
build/register_layout.o: checks/register_layout.c | build
>$(CC) $(CFLAGS) -c $< -o $@
check: all build/register_layout.o
>$(CC) $(LDFLAGS) -Wl,--no-gc-sections,-Map=build/all-drivers.map $(OBJECTS) -lgcc -o build/all-drivers.elf
check-host: | build
>$(HOSTCC) -std=c11 -Wall -Wextra -Werror -Iinclude -Isrc checks/test_deadline.c -o build/test_deadline
>./build/test_deadline
flash: build/firmware.elf
>openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/firmware.elf verify reset exit"
clean:
>rm -rf build
-include $(OBJECTS:.o=.d) build/register_layout.d
