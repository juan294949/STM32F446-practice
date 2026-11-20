#include "gpio.h"
#include "stm32f446/regs/gpio.h"
void gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;
    GPIOA->BSRR = 1u << 21;
    GPIOA->MODER = (GPIOA->MODER & ~(3u << 10)) | (1u << 10);
    GPIOA->OTYPER &= ~(1u << 5);
    GPIOA->OSPEEDR &= ~(3u << 10);
    GPIOA->PUPDR &= ~(3u << 10);
    GPIOC->MODER &= ~(3u << 26);
    GPIOC->PUPDR = (GPIOC->PUPDR & ~(3u << 26)) |
                   ((BOARD_BUTTON_ACTIVE_HIGH ? 2u : 1u) << 26);
}
void led_write(bool on) { GPIOA->BSRR = on ? (1u << 5) : (1u << 21); }
void led_toggle(void) { led_write((GPIOA->ODR & (1u << 5)) == 0u); }
bool button_raw_level(void) { return (GPIOC->IDR & (1u << 13)) != 0u; }
bool button_pressed(void)
{
    return BOARD_BUTTON_ACTIVE_HIGH ? button_raw_level() : !button_raw_level();
}
