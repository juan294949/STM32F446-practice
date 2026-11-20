#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H
#include <stdint.h>
#define CORE_HZ 16000000u
#define APB1_HZ CORE_HZ
#define APB2_HZ CORE_HZ
/* Boot-time HSI initialization; not an arbitrary runtime clock switch. */
void clock_init(void);
#endif
