#include <stddef.h>
#include "device_info.h"
#include "stm32f446/device.h"
uint16_t device_flash_kib(void) { return *(volatile const uint16_t *)FLASHSIZE_BASE; }
uint16_t device_part_id(void) { return (uint16_t)(DBGMCU->IDCODE & 0xFFFu); }
uint16_t device_revision_id(void) { return (uint16_t)(DBGMCU->IDCODE >> 16); }
bool device_read_uid(uint32_t out[3])
{
    if (out == NULL) return false;
    volatile const uint32_t *const uid = (volatile const uint32_t *)UID_BASE;
    for (unsigned i = 0u; i < 3u; ++i) out[i] = uid[i];
    return true;
}
