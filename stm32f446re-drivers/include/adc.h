#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H
#include <stdint.h>
#include "driver_status.h"
/* ADC1 PA0/channel 0, 12-bit, single conversion, 480-cycle sample.
   Requires clock_init() and time_init(); owns ADC common prescaler.
   ADC2/3 must not be active during initialization. Input within VSSA..VDDA. */
void adc1_init_pa0(void);
/* On timeout/error, reinitialize ADC1 before retrying. */
driver_status_t adc1_read(uint16_t *sample, uint32_t timeout_ms);
/* sample 0..4095; reference_mv should be the measured ADC reference. */
uint32_t adc12_to_millivolts(uint16_t sample, uint16_t reference_mv);
#endif
