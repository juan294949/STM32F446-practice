#include <assert.h>
#include <stdint.h>
#include "driver_wait.h"
static uint32_t simulated_ms;
uint32_t millis(void) { return simulated_ms; }
int main(void)
{
    simulated_ms = 123u;
    driver_deadline_t d = driver_deadline(0u);
    assert(driver_expired(d));
    d = driver_deadline(10u);
    simulated_ms = 132u; assert(!driver_expired(d));
    simulated_ms = 133u; assert(driver_expired(d));
    simulated_ms = UINT32_MAX - 5u; d = driver_deadline(10u);
    simulated_ms = 3u; assert(!driver_expired(d));
    simulated_ms = 4u; assert(driver_expired(d));
    return 0;
}
