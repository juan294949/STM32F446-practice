#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H
#include <stdbool.h>
#ifndef BOARD_BUTTON_ACTIVE_HIGH
#define BOARD_BUTTON_ACTIVE_HIGH 0
#endif
/* PA5 LD2 output, PC13 B1 input. */
void gpio_init(void);
void led_write(bool on);
/* Single caller/context only: read followed by write. */
void led_toggle(void);
bool button_raw_level(void);
/* Default active-low; override BOARD_BUTTON_ACTIVE_HIGH if needed.
   The interactive test instead learns the released level at startup. */
bool button_pressed(void);
#endif
