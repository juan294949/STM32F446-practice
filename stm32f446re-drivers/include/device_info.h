#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H
#include <stdint.h>
#include <stdbool.h>
uint16_t device_flash_kib(void);
uint16_t device_part_id(void);
uint16_t device_revision_id(void);
/* Output buffer must have room for three words. */
bool device_read_uid(uint32_t out[3]);
#endif
