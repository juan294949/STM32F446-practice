# Validation

Arm GNU GCC 12.2.1; Cortex-M4 Thumb soft-float; -Wall -Wextra -Werror.

Passed:
- make check: all drivers, target tests, register-layout assertions and full link.
- make check-host: zero/exact timeout boundaries and unsigned counter wrap.
- All 29 peripheral register entry headers compiled independently.
- Full-driver ELF has no undefined symbols.

Default application image: 5,696 bytes flash. Static BSS: 20 bytes; reserved
stack: 4,096 bytes. No compiled images or archives are included in the commit.

No physical target or peripheral fixture was connected. Hardware test functions
are implemented but have not been executed on a board. Build success does not
establish SPI/I2C signal correctness, ADC accuracy, USB serial wiring or timing.
