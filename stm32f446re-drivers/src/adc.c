#include <stdbool.h>
#include <stddef.h>
#include "adc.h"
#include "driver_wait.h"
#include "stm32f446/regs/adc.h"
static bool initialized;
void adc1_init_pa0(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    (void)RCC->APB2ENR;
    GPIOA->MODER |= 3u;
    GPIOA->PUPDR &= ~3u;
    ADC1->CR2 = 0u; ADC1->CR1 = 0u;
    ADC123_COMMON->CCR = (ADC123_COMMON->CCR & ~(ADC_CCR_ADCPRE | ADC_CCR_MULTI)) |
                         ADC_CCR_ADCPRE_0; /* APB2 / 4 = 4 MHz, independent ADC. */
    ADC1->SMPR2 = (ADC1->SMPR2 & ~ADC_SMPR2_SMP0) | ADC_SMPR2_SMP0;
    ADC1->SQR1 = 0u; ADC1->SQR2 = 0u; ADC1->SQR3 = 0u;
    ADC1->SR = 0u;
    ADC1->CR2 = ADC_CR2_ADON;
    delay_ms(1u);
    initialized = true;
}
driver_status_t adc1_read(uint16_t *sample, uint32_t timeout_ms)
{
    if (sample == NULL) return DRIVER_INVALID_ARGUMENT;
    if (!initialized) return DRIVER_NOT_INITIALIZED;
    const driver_deadline_t d = driver_deadline(timeout_ms);
    (void)ADC1->DR;
    ADC1->SR = 0u;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    for (;;) {
        const uint32_t sr = ADC1->SR;
        if (sr & ADC_SR_OVR) {
            ADC1->CR2 &= ~ADC_CR2_ADON; initialized = false; return DRIVER_IO_ERROR;
        }
        if (sr & ADC_SR_EOC) { *sample = (uint16_t)(ADC1->DR & 0xFFFu); return DRIVER_OK; }
        if (driver_expired(d)) {
            ADC1->CR2 &= ~ADC_CR2_ADON; initialized = false; return DRIVER_TIMEOUT;
        }
    }
}
uint32_t adc12_to_millivolts(uint16_t sample, uint16_t reference_mv)
{
    return ((uint32_t)sample * reference_mv + 2047u) / 4095u;
}
