#include "driver_tests.h"
#include "device_info.h"
#include "uart.h"
test_result_t test_device_info(void)
{
    uint32_t uid[3]; const uint16_t flash = device_flash_kib();
    uart_write("Flash KiB: "); uart_write_u32(flash);
    uart_write(" | device ID (decimal): "); uart_write_u32(device_part_id());
    uart_write(" | revision (decimal): "); uart_write_u32(device_revision_id());
    uart_write("\r\nUID words (decimal): ");
    if (!device_read_uid(uid)) return TEST_FAIL;
    for (unsigned i = 0u; i < 3u; ++i) { uart_write_u32(uid[i]); uart_write(" "); }
    uart_write("\r\nAutomatic check: F446RE flash size must be 512 KiB.\r\n");
    return flash == 512u ? TEST_PASS : TEST_FAIL;
}
