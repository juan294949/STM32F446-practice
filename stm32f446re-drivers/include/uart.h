#ifndef UART_DRIVER_H
#define UART_DRIVER_H
#include <stdint.h>
#include <stdbool.h>
/* USART2 PA2/PA3 AF7, 115200 baud, 8N1. */
void uart_init(void);
/* Blocking TX, foreground only; no timeout. */
void uart_putc(char c);
/* Non-null, NUL-terminated string; no automatic newline conversion. */
void uart_write(const char *text);
void uart_write_u32(uint32_t value);
/* Nonblocking RX. Valid out pointer required; errored bytes dropped.
   No software queue: poll often. */
bool uart_try_getc(char *out);
#endif
