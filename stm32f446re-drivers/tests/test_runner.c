#include "driver_tests.h"
#include "uart.h"
void driver_tests_run(const driver_test_case_t *tests, size_t count)
{
    unsigned passed = 0u, failed = 0u, observe = 0u, skipped = 0u;
    for (size_t i = 0u; i < count; ++i) {
        uart_write("\r\n[TEST] "); uart_write(tests[i].name); uart_write("\r\n");
        if (!tests[i].enabled) {
            ++skipped; uart_write("[SKIP] Disabled in test_config.h\r\n"); continue;
        }
        switch (tests[i].run()) {
        case TEST_PASS: ++passed; uart_write("[PASS]\r\n"); break;
        case TEST_FAIL: ++failed; uart_write("[FAIL]\r\n"); break;
        case TEST_OBSERVE: ++observe; uart_write("[OBSERVE] Check physical/terminal behavior\r\n"); break;
        }
    }
    uart_write("\r\nSummary: PASS="); uart_write_u32(passed);
    uart_write(" FAIL="); uart_write_u32(failed);
    uart_write(" OBSERVE="); uart_write_u32(observe);
    uart_write(" SKIP="); uart_write_u32(skipped); uart_write("\r\n");
}
