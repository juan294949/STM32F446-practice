# STM32F446RE bare-metal drivers and tests

This is a self-contained Arm GNU C project for STM32F446RE / NUCLEO-F446RE.
It lives separately from the repository's earlier experiments. All source,
register definitions, startup code and linker files are present; no download
script, HAL, RTOS, IDE project generator or archive is required.

## Start here

- `src/main.c`: initialization, a list of tests, and the interactive loop.
- `include/`: public driver interfaces and their calling contracts.
- `src/`: one implementation per driver.
- `tests/`: one-shot driver tests and interactive button/UART checks.
- `include/test_config.h`: enable tests that require external wiring.
- `include/stm32f446/regs/`: peripheral entry headers for the official definitions.
- `vendor/`: pinned ST device and Arm CMSIS core headers, licenses and provenance.
- `checks/`: compile-time layout checks and a host-side timeout test.

Clock configuration is internal HSI at 16 MHz, AHB/APB1/APB2 divide by one.
These drivers assume that configuration. Call clock_init() at boot, then
initialize timebase and any peripherals you use. They are foreground polling
drivers, not interrupt-driven, DMA-driven, reentrant, or complete protocol stacks.

## Interfaces and functions

| Driver / files | Public functions |
| --- | --- |
| clock.c / clock.h | clock_init |
| gpio.c / gpio.h | gpio_init, led_write, led_toggle, button_raw_level, button_pressed |
| uart.c / uart.h | uart_init, uart_putc, uart_write, uart_write_u32, uart_try_getc |
| timebase.c / timebase.h | time_init, millis, delay_ms, SysTick_Handler |
| spi.c / spi.h | spi1_init, spi1_select, spi1_transfer, spi1_transaction, spi1_deinit |
| i2c.c / i2c.h | i2c1_init, i2c1_write, i2c1_read, i2c1_write_read |
| adc.c / adc.h | adc1_init_pa0, adc1_read, adc12_to_millivolts |
| timer.c / timer.h | timer2_init_1mhz, timer2_micros, timer2_elapsed, timer2_delay_us, timer3_periodic_start, timer3_period_elapsed, timer3_stop |
| device_info.c / device_info.h | device_flash_kib, device_part_id, device_revision_id, device_read_uid |

The complete ST STM32F446 header supplies peripheral structures, base addresses,
register bit masks and interrupt numbers. Arm headers supply Cortex-M4 NVIC,
SysTick, SCB, DWT and MPU definitions. The per-interface headers deliberately
include the same shared definitions instead of duplicating register layouts.
Register definitions belong in .h files; driver functions belong in .c files.
Some family-header peripherals/pins are unavailable in the RE package; consult
the LQFP64 datasheet pinout and your exact board schematic.

Example register access:

```c
#include "stm32f446/regs/rcc.h"
#include "stm32f446/regs/gpio.h"
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
(void)RCC->AHB1ENR;
GPIOA->BSRR = 1u << 5; /* Assumes PA5 was configured as output. */
```

## Build, compile and link

Install Arm GNU Toolchain, GNU Make, and optionally OpenOCD. Debian/Ubuntu:

```sh
sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi make openocd
cd stm32f446re-drivers
make -j
make check
```

If the toolchain is outside PATH:

```sh
make PREFIX=/absolute/toolchain/bin/arm-none-eabi- check
```

Run the laptop-side timeout test if a host C compiler is installed:

```sh
make check-host
```

The Makefile is authoritative. Equivalent explicit Bash compilation/linking:

```sh
mkdir -p build
for source in src/*.c tests/*.c; do
    name=$(basename "$source" .c)
    case "$source" in tests/*) name="tests_$name" ;; esac
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
      -std=gnu11 -Os -g3 -Wall -Wextra -Werror \
      -ffreestanding -fno-builtin -fno-common \
      -ffunction-sections -fdata-sections -DSTM32F446xx \
      -Iinclude -Ivendor/ST/Include -Ivendor/CMSIS/Include \
      -c "$source" -o "build/$name.o"
done
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
  -nostdlib -nostartfiles -TSTM32F446RE.ld \
  -Wl,--gc-sections,--build-id=none,-Map=build/firmware.map \
  build/adc.o build/clock.o build/device_info.o build/gpio.o \
  build/i2c.o build/main.o build/spi.o build/startup.o \
  build/timebase.o build/timer.o build/uart.o build/tests_*.o \
  -lgcc -o build/firmware.elf
arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin
arm-none-eabi-objcopy -O ihex build/firmware.elf build/firmware.hex
arm-none-eabi-size build/firmware.elf
```

Compilation creates an object for each .c file. Linking resolves functions,
keeps vectors/code/constants in flash, and places variables in RAM. Reset copies
.data and zeros .bss before main. The memory map is 512 KiB flash at 0x08000000,
128 KiB RAM at 0x20000000, and a reserved 4 KiB stack with initial SP 0x20020000.
The linker catches static RAM/stack overlap; runtime stack overflow is not
monitored. There is no heap or libc. -lgcc supplies compiler helpers. The
soft-float ABI does not require enabling the FPU. C++ constructors are not
supported by this startup.

Outputs stay in ignored build/: ELF, BIN, HEX, map and dependencies. make check
also compiles register-layout assertions and links all functions without garbage
collection to expose otherwise hidden missing symbols. Test-table pointers can
retain driver functions even when a test is disabled. Flash firmware.elf, not
the auxiliary all-drivers.elf produced by the link check.

## Run tests on the board

Connect the Nucleo ST-LINK USB connector and run:

```sh
make flash
```

Alternatively program build/firmware.hex using STM32CubeProgrammer. For the raw
BIN use base address 0x08000000. Flashing replaces the current application.

Open the ST-LINK virtual COM port at 115200 baud, 8 data bits, no parity, 1 stop
bit, no flow control. The startup banner is `STM32F446RE DRIVER TESTS V1`.
UART transmit blocks without a timeout. Check board UART solder bridges if the
terminal is silent; the build checks do not verify your USB/serial wiring.

Default tests:

1. UART transmits a readable character pattern (OBSERVE).
2. LD2 flashes three times and the button released level is recorded (OBSERVE).
   **Leave B1 released during these flashes.**
3. Flash size must report 512 KiB; device ID/revision and UID are printed.
4. TIM2 counter advancement is compared against a SysTick delay.
5. TIM3 must produce a 10 ms periodic event.
6. SPI, I2C and ADC fixture tests print SKIP until explicitly enabled.

After the list finishes, holding B1 turns LD2 on and releasing turns it off.
PC13's released level is learned, so this interaction does not assume button
polarity. The standalone button_pressed() helper defaults to active low and can
be configured with BOARD_BUTTON_ACTIVE_HIGH in gpio.h. GPIO pull direction also
follows that setting; check your actual board. Typing terminal keys prints their
received decimal byte values.

PASS means the named automatic check succeeded, not full hardware qualification.
OBSERVE means you must check physical/terminal behavior. FAIL is an unmet check;
SKIP is intentionally disabled. Timer comparisons use the same oscillator and
are not independent frequency calibration.

## Optional fixtures and pin ownership

| Interface | Pins/resources | Setup and limitations |
| --- | --- | --- |
| GPIO | PA5 LD2, PC13 B1 | Active-high LED; interactive test learns button idle level |
| USART2 | PA2 TX, PA3 RX AF7 | Normally routed through ST-LINK VCP; no RX software queue |
| SPI1 | PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI AF5 | 8-bit MSB-first master, modes 0..3 |
| I2C1 | PB8 SCL, PB9 SDA AF4 open drain | 100 kHz, external pull-ups to 3.3 V, single master |
| ADC1 | PA0 / channel 0 | 12-bit single conversion; 480-cycle sample; ADC clock APB2/4 |
| TIM2 | No pins | Free-running 1 MHz 32-bit counter |
| TIM3 | No pins | Polled 1..65536 ms periodic event |

Change only the fixtures you have in include/test_config.h:

- TEST_ENABLE_SPI_LOOPBACK=1: wire PA7 to PA6. PA5 is also LD2, so the test
  temporarily owns it as SPI clock. It resets SPI1 and restores GPIO afterward.
  Do not call LED functions while your own application is using SPI1 on PA5.
- TEST_ENABLE_I2C_SENSOR=1: defaults describe a 3.3 V compatible MPU6050 at
  address 0x68, WHO_AM_I register 0x75, expected value 0x68. Configure
  TEST_I2C_ADDRESS/REGISTER/EXPECTED for your actual target. Fit appropriate
  external pull-ups on PB8/PB9. These values are not universal sensor support.
- TEST_ENABLE_ADC_GROUND=1: wire PA0 to GND. The test accepts <=32 ADC counts.
  Input voltage must stay within VSSA..VDDA. Do not apply 5 V to an ADC input.

SPI clock divisors are 2..256; at 16 MHz APB2 the minimum supported request is
62500 Hz. spi1_transfer() leaves CS alone; spi1_transaction() handles active-low
PA4 CS. Null TX sends 0xff; null RX discards received bytes.

I2C uses unshifted 7-bit addresses 0x08..0x77. write_read() uses a repeated START.
RX sequences distinguish 1, 2 and >=3 bytes. Zero-length requests are rejected.
There is no 10-bit addressing, slave mode, DMA, automatic retries or GPIO-pulse
bus recovery. Physical bus recovery may require resetting the target.

ADC initialization owns the common prescaler/mode; ADC2 and ADC3 must not be
running concurrently. Millivolt conversion requires a valid 0..4095 sample and
the measured reference voltage in millivolts, not an assumed exact 3300 mV.

## Calling rules and error handling

Initialize first and serialize calls in the foreground. Do not share owned
peripherals with other drivers, DMA or interrupts. SPI/I2C/ADC deadlines use
millis(), so call time_init() and leave interrupts enabled. A single timeout
covers each whole transfer. A zero timeout checks ready conditions without
waiting. Read buffers may be partially filled on failure.

New drivers use driver_status_t: OK, INVALID_ARGUMENT, NOT_INITIALIZED, BUSY,
TIMEOUT, IO_ERROR, NACK and ARBITRATION_LOST. After I2C failure reinitialize
before retrying; reinitialization cannot free SDA held by external hardware.
After SPI timeout/IO_ERROR release CS and reinitialize. ADC timeout/overrun powers
down ADC1; initialize again before retrying.

millis() wraps every ~49.7 days; TIM2's microsecond counter wraps every ~71.6
minutes. Unsigned subtraction handles wrap when observed within one full wrap
interval. TIM2 busy delays require prior timer2_init_1mhz() but no interrupts.
TIM3's update flag coalesces missed periods; it is not an event counter. No PWM,
capture or timer IRQ configuration is supplied by this driver.

External interrupt vectors currently trap in Default_Handler. Assign a real
vector and configure/clear peripheral flags before enabling an NVIC interrupt.
No selected peripheral driver requires extra IRQ vectors.

## Provenance and validation

vendor/PROVENANCE.json pins the ST device header to an exact commit and Arm
CMSIS_5 to 5.9.0, with source URLs and SHA-256 hashes. Vendor licenses and original
notices are preserved. No HAL source or prebuilt firmware is checked in.

See VALIDATION.md for checks actually run. No physical board or fixtures were
attached during these checks. SPI waveforms, I2C receive end sequences, ADC
accuracy, electrical setup and timing require validation on your hardware.
Consult ST RM0390, device errata, the F446RE datasheet and your board schematic.
