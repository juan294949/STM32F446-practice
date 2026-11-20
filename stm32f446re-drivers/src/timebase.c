#include "timebase.h"
#include "clock.h"
#include "stm32f446/regs/core.h"
static volatile uint32_t ticks;
void time_init(void)
{
    SysTick->CTRL = 0u;
    ticks = 0u;
    SysTick->LOAD = CORE_HZ / 1000u - 1u;
    SysTick->VAL = 0u;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}
void SysTick_Handler(void) { ++ticks; }
uint32_t millis(void) { return ticks; }
void delay_ms(uint32_t duration)
{
    const uint32_t start = millis();
    while ((uint32_t)(millis() - start) < duration) __WFI();
}
