#ifndef DRIVER_TESTS_H
#define DRIVER_TESTS_H
#include <stdbool.h>
#include <stddef.h>
typedef enum { TEST_PASS, TEST_FAIL, TEST_OBSERVE } test_result_t;
typedef struct {
    const char *name;
    test_result_t (*run)(void);
    bool enabled;
} driver_test_case_t;
void driver_tests_run(const driver_test_case_t *tests, size_t count);
test_result_t test_device_info(void);
test_result_t test_uart(void);
test_result_t test_gpio(void);
test_result_t test_timer2(void);
test_result_t test_timer3(void);
test_result_t test_spi(void);
test_result_t test_i2c(void);
test_result_t test_adc(void);
void test_button_poll(void);
void test_uart_poll(void);
#endif
