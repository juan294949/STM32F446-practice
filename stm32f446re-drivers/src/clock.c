#include "clock.h"
#include "stm32f446/regs/rcc.h"
void clock_init(void)
{
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) {}
    RCC->CFGR &= ~RCC_CFGR_SW;
    while (RCC->CFGR & RCC_CFGR_SWS) {}
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON);
}
