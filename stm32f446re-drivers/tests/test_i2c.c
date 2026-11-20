#include "driver_tests.h"
#include "i2c.h"
#include "test_config.h"
#include "uart.h"
test_result_t test_i2c(void)
{
    const uint8_t reg = TEST_I2C_REGISTER; uint8_t value = 0u;
    i2c1_init();
    const driver_status_t s = i2c1_write_read(TEST_I2C_ADDRESS, &reg, 1u, &value, 1u, 100u);
    uart_write("I2C status code: "); uart_write_u32(s);
    uart_write(" | register value (decimal): "); uart_write_u32(value); uart_write("\r\n");
    return s == DRIVER_OK && value == TEST_I2C_EXPECTED ? TEST_PASS : TEST_FAIL;
}
