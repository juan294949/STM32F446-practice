#include "driver_tests.h"
#include "adc.h"
#include "uart.h"
test_result_t test_adc(void)
{
    uart_write("ADC ground test: PA0 must be connected to GND.\r\n");
    adc1_init_pa0();
    uint16_t sample = 0u;
    const driver_status_t s = adc1_read(&sample, 20u);
    uart_write("ADC status code: "); uart_write_u32(s);
    uart_write(" | raw sample: "); uart_write_u32(sample); uart_write("\r\n");
    return s == DRIVER_OK && sample <= 32u ? TEST_PASS : TEST_FAIL;
}
