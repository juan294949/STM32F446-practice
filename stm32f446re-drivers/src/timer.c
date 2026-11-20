#include "timer.h"
#include "clock.h"
#include "stm32f446/regs/tim.h"
static bool timer2_ready;
static bool timer3_ready;
void timer2_init_1mhz(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; (void)RCC->APB1ENR;
    TIM2->CR1 = 0u; TIM2->CR2 = 0u; TIM2->SMCR = 0u; TIM2->DIER = 0u;
    TIM2->PSC = APB1_HZ / 1000000u - 1u;
    TIM2->ARR = UINT32_MAX; TIM2->CNT = 0u;
    TIM2->EGR = TIM_EGR_UG; TIM2->SR = 0u;
    TIM2->CR1 = TIM_CR1_URS | TIM_CR1_CEN;
    timer2_ready = true;
}
uint32_t timer2_micros(void) { return timer2_ready ? TIM2->CNT : 0u; }
bool timer2_elapsed(uint32_t start, uint32_t interval_us)
{
    return timer2_ready && (uint32_t)(timer2_micros() - start) >= interval_us;
}
driver_status_t timer2_delay_us(uint32_t duration_us)
{
    if (!timer2_ready) return DRIVER_NOT_INITIALIZED;
    const uint32_t start = timer2_micros();
    while (!timer2_elapsed(start, duration_us)) {}
    return DRIVER_OK;
}
driver_status_t timer3_periodic_start(uint32_t period_ms)
{
    if (!period_ms || period_ms > 65536u) return DRIVER_INVALID_ARGUMENT;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; (void)RCC->APB1ENR;
    TIM3->CR1 = 0u; TIM3->CR2 = 0u; TIM3->SMCR = 0u; TIM3->DIER = 0u;
    TIM3->PSC = APB1_HZ / 1000u - 1u;
    TIM3->ARR = period_ms - 1u; TIM3->CNT = 0u;
    TIM3->EGR = TIM_EGR_UG; TIM3->SR = 0u;
    TIM3->CR1 = TIM_CR1_URS | TIM_CR1_CEN;
    timer3_ready = true;
    return DRIVER_OK;
}
bool timer3_period_elapsed(void)
{
    if (!timer3_ready || !(TIM3->SR & TIM_SR_UIF)) return false;
    TIM3->SR = ~TIM_SR_UIF;
    return true;
}
void timer3_stop(void)
{
    if (timer3_ready) TIM3->CR1 &= ~TIM_CR1_CEN;
    timer3_ready = false;
}
