#include "driver_tests.h"
#include "uart.h"
test_result_t test_uart(void)
{
    uart_write("UART2 TX: ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789\r\n");
    uart_write("Terminal: 115200 baud, 8N1, no flow control.\r\n");
    uart_write("After tests type a key to see its decimal byte value.\r\n");
    return TEST_OBSERVE;
}
void test_uart_poll(void)
{
    char c;
    if (uart_try_getc(&c)) {
        uart_write("[UART RX] byte="); uart_write_u32((unsigned char)c); uart_write("\r\n");
    }
}
