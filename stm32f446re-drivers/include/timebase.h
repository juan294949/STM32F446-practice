#ifndef TIMEBASE_DRIVER_H
#define TIMEBASE_DRIVER_H
#include <stdint.h>
/* Call after clock_init(), once during initialization. */
void time_init(void);
/* Milliseconds, wraps every ~49.7 days. */
uint32_t millis(void);
/* Foreground only, interrupts enabled. */
void delay_ms(uint32_t duration);
void SysTick_Handler(void);
#endif
