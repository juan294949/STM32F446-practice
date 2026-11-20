#include "driver_tests.h"
#include "timer.h"
#include "timebase.h"
#include "uart.h"
test_result_t test_timer2(void)
{
    timer2_init_1mhz();
    const uint32_t start = timer2_micros();
    delay_ms(10u);
    const uint32_t elapsed = (uint32_t)(timer2_micros() - start);
    uart_write("TIM2 microseconds during SysTick 10 ms wait: ");
    uart_write_u32(elapsed); uart_write("\r\n");
    /* Relative check only: both counters use the same oscillator. */
    return elapsed >= 8000u && elapsed <= 14000u ? TEST_PASS : TEST_FAIL;
}
test_result_t test_timer3(void)
{
    if (timer3_periodic_start(10u) != DRIVER_OK) return TEST_FAIL;
    const uint32_t start = millis();
    while (!timer3_period_elapsed()) {
        if ((uint32_t)(millis() - start) >= 100u) { timer3_stop(); return TEST_FAIL; }
    }
    const uint32_t elapsed = (uint32_t)(millis() - start);
    timer3_stop();
    uart_write("TIM3 10 ms event observed after ms: ");
    uart_write_u32(elapsed); uart_write("\r\n");
    return elapsed >= 8u && elapsed <= 20u ? TEST_PASS : TEST_FAIL;
}
