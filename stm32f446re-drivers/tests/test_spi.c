#include "driver_tests.h"
#include "spi.h"
#include "gpio.h"
#include "uart.h"
test_result_t test_spi(void)
{
    const uint8_t tx[] = { 0x00u, 0x55u, 0xAAu, 0xFFu };
    uint8_t rx[sizeof(tx)];
    uart_write("SPI1 loopback: PA7 to PA6; PA5 temporarily becomes SPI clock.\r\n");
    driver_status_t s = spi1_init(1000000u, 0u);
    if (s == DRIVER_OK) s = spi1_transaction(tx, rx, sizeof(tx), 100u);
    bool match = s == DRIVER_OK;
    if (match) for (unsigned i = 0u; i < sizeof(tx); ++i) if (rx[i] != tx[i]) match = false;
    spi1_deinit();
    gpio_init(); /* Restore PA5 LED ownership. */
    uart_write("SPI status code: "); uart_write_u32(s); uart_write("\r\n");
    return match ? TEST_PASS : TEST_FAIL;
}
