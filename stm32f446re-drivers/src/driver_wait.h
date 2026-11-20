#ifndef DRIVER_WAIT_H
#define DRIVER_WAIT_H
#include <stdint.h>
#include <stdbool.h>
#include "timebase.h"
/* Entire-transfer deadline. Requires running SysTick and interrupts enabled. */
typedef struct { uint32_t start; uint32_t timeout; } driver_deadline_t;
static inline driver_deadline_t driver_deadline(uint32_t timeout_ms)
{
    driver_deadline_t d = { millis(), timeout_ms };
    return d;
}
static inline bool driver_expired(driver_deadline_t d)
{
    return (uint32_t)(millis() - d.start) >= d.timeout;
}
#endif
