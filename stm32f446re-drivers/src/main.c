#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "timebase.h"
#include "driver_tests.h"
#include "test_config.h"
int main(void)
{
    clock_init();
    gpio_init();
    time_init();
    uart_init();
    uart_write("\r\nSTM32F446RE DRIVER TESTS V1\r\n");
    static const driver_test_case_t tests[] = {
        { "UART transmit / receive instructions", test_uart, true },
        { "LED flashes and button setup", test_gpio, true },
        { "Device registers and 512 KiB flash", test_device_info, true },
        { "TIM2 microsecond counter", test_timer2, true },
        { "TIM3 periodic event", test_timer3, true },
        { "SPI1 MOSI-MISO loopback", test_spi, TEST_ENABLE_SPI_LOOPBACK },
        { "I2C1 sensor register read", test_i2c, TEST_ENABLE_I2C_SENSOR },
        { "ADC1 PA0 connected to GND", test_adc, TEST_ENABLE_ADC_GROUND },
    };
    driver_tests_run(tests, sizeof(tests) / sizeof(tests[0]));
    uart_write("\r\nInteractive: hold B1 for LD2; type keys for UART RX.\r\n");
    for (;;) {
        test_button_poll();
        test_uart_poll();
        delay_ms(5u);
    }
}
