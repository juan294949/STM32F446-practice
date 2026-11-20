#include <stdint.h>
#include "timebase.h"
#include "stm32f446/device.h"
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);
void Reset_Handler(void);
void Default_Handler(void);
#define WEAK_HANDLER(name) void name(void) __attribute__((weak, alias("Default_Handler")))
WEAK_HANDLER(NMI_Handler);
WEAK_HANDLER(HardFault_Handler);
WEAK_HANDLER(MemManage_Handler);
WEAK_HANDLER(BusFault_Handler);
WEAK_HANDLER(UsageFault_Handler);
WEAK_HANDLER(SVC_Handler);
WEAK_HANDLER(DebugMon_Handler);
WEAK_HANDLER(PendSV_Handler);
/* GNU range designator. External IRQs trap until their vectors are assigned. */
__attribute__((section(".isr_vector"), used, aligned(512)))
void (*const vectors[113])(void) = {
    [0] = (void (*)(void))(&_estack), [1] = Reset_Handler,
    [2] = NMI_Handler, [3] = HardFault_Handler,
    [4] = MemManage_Handler, [5] = BusFault_Handler, [6] = UsageFault_Handler,
    [11] = SVC_Handler, [12] = DebugMon_Handler,
    [14] = PendSV_Handler, [15] = SysTick_Handler,
    [16 ... 112] = Default_Handler
};
void Reset_Handler(void)
{
    __disable_irq();
    SCB->VTOR = (uint32_t)(uintptr_t)vectors;
    __DSB(); __ISB();
    for (uint32_t *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;
    for (uint32_t *dst = &_sbss; dst < &_ebss;) *dst++ = 0u;
    __enable_irq();
    (void)main();
    Default_Handler();
}
void Default_Handler(void)
{
    __disable_irq();
    for (;;) __WFI();
}
