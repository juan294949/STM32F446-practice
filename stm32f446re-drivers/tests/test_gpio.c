#include "driver_tests.h"
#include "gpio.h"
#include "uart.h"
#include "timebase.h"
static bool released_level, candidate, pressed;
static uint32_t changed_at;
test_result_t test_gpio(void)
{
    uart_write("Keep B1 RELEASED during the three LED flashes.\r\n");
    for (unsigned i = 0u; i < 3u; ++i) {
        led_write(true); delay_ms(200u);
        led_write(false); delay_ms(200u);
    }
    delay_ms(50u);
    released_level = button_raw_level();
    candidate = false; pressed = false; changed_at = millis();
    uart_write("Released PC13 level: ");
    uart_write(released_level ? "HIGH\r\n" : "LOW\r\n");
    uart_write("After tests: hold B1 for solid LD2; release for OFF.\r\n");
    return TEST_OBSERVE;
}
void test_button_poll(void)
{
    const uint32_t now = millis();
    const bool sample = button_raw_level() != released_level;
    if (sample != candidate) { candidate = sample; changed_at = now; }
    if (candidate != pressed && (uint32_t)(now - changed_at) >= 30u) {
        pressed = candidate;
        uart_write(pressed ? "[BUTTON] PRESSED\r\n" : "[BUTTON] RELEASED\r\n");
    }
    led_write(pressed);
}
