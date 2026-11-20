#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "driver_status.h"
/* SPI1 master: PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI. PA5 conflicts with LD2.
   8-bit MSB-first, mode 0..3, fastest divisor not exceeding max_hz.
   Owns SPI1 and these pins. Requires clock_init() at 16 MHz. */
driver_status_t spi1_init(uint32_t max_hz, unsigned mode);
void spi1_select(bool selected);
/* Null TX sends 0xff; null RX discards data. Leaves CS unchanged.
   Timeout covers the entire call; time_init() and interrupts required.
   After timeout/IO_ERROR, release CS and reinitialize before retrying. */
driver_status_t spi1_transfer(const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms);
driver_status_t spi1_transaction(const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms);
/* Abort/reset SPI1 and release CS; does not restore GPIO modes. */
void spi1_deinit(void);
#endif
