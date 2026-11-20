#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H
#include <stddef.h>
#include <stdint.h>
#include "driver_status.h"
/* I2C1 master, 100 kHz, PB8 SCL / PB9 SDA AF4 open drain.
   External pull-ups to 3.3 V required. Unshifted 7-bit addresses 0x08..0x77.
   Requires clock_init() and time_init(). Foreground, single-master only.
   Nonzero lengths, valid pointers; no DMA, slave mode or bus recovery.
   After any failure call i2c1_init() before retrying. Reinitialization does
   not free a bus held by external hardware. One timeout covers a whole call. */
void i2c1_init(void);
driver_status_t i2c1_write(uint8_t address, const uint8_t *data, size_t length, uint32_t timeout_ms);
driver_status_t i2c1_read(uint8_t address, uint8_t *data, size_t length, uint32_t timeout_ms);
/* Repeated START without an intermediate STOP. */
driver_status_t i2c1_write_read(uint8_t address, const uint8_t *tx, size_t tx_length,
                               uint8_t *rx, size_t rx_length, uint32_t timeout_ms);
#endif
