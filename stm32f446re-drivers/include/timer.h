#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H
#include <stdint.h>
#include <stdbool.h>
#include "driver_status.h"
/* Requires APB1 timer input 16 MHz. Owns TIM2/TIM3, uses no pins or IRQs. */
void timer2_init_1mhz(void);
/* Wraps approximately every 71.6 minutes. */
uint32_t timer2_micros(void);
bool timer2_elapsed(uint32_t start, uint32_t interval_us);
driver_status_t timer2_delay_us(uint32_t duration_us);
/* 1..65536 ms; slow polling coalesces events. */
driver_status_t timer3_periodic_start(uint32_t period_ms);
bool timer3_period_elapsed(void);
void timer3_stop(void);
#endif
