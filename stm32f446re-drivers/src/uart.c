#include "uart.h"
#include "clock.h"
#include "stm32f446/regs/usart.h"
void uart_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    (void)RCC->APB1ENR;
    GPIOA->MODER = (GPIOA->MODER & ~((3u << 4) | (3u << 6))) |
                   (2u << 4) | (2u << 6);
    GPIOA->AFR[0] = (GPIOA->AFR[0] & ~((15u << 8) | (15u << 12))) |
                    (7u << 8) | (7u << 12);
    GPIOA->OTYPER &= ~((1u << 2) | (1u << 3));
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~((3u << 4) | (3u << 6))) |
                     (1u << 4) | (1u << 6);
    GPIOA->PUPDR = (GPIOA->PUPDR & ~((3u << 4) | (3u << 6))) | (1u << 6);
    USART2->CR1 = 0u; USART2->CR2 = 0u; USART2->CR3 = 0u;
    USART2->BRR = (APB1_HZ + 115200u / 2u) / 115200u;
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}
void uart_putc(char c)
{
    while (!(USART2->SR & USART_SR_TXE)) {}
    USART2->DR = (uint8_t)c;
}
void uart_write(const char *text) { while (*text) uart_putc(*text++); }
void uart_write_u32(uint32_t value)
{
    char digits[10]; unsigned count = 0u;
    do { digits[count++] = (char)('0' + value % 10u); value /= 10u; } while (value);
    while (count) uart_putc(digits[--count]);
}
bool uart_try_getc(char *out)
{
    const uint32_t status = USART2->SR;
    if (!(status & (USART_SR_RXNE | 15u))) return false;
    const uint32_t data = USART2->DR; /* SR then DR clears error flags. */
    if ((status & 15u) || !(status & USART_SR_RXNE)) return false;
    *out = (char)(data & 0xFFu);
    return true;
}
