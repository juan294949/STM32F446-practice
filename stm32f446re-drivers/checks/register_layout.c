#include <stddef.h>
#include "stm32f446/device.h"
_Static_assert(offsetof(GPIO_TypeDef, IDR) == 0x10u, "GPIO IDR layout");
_Static_assert(offsetof(GPIO_TypeDef, BSRR) == 0x18u, "GPIO BSRR layout");
_Static_assert(offsetof(RCC_TypeDef, AHB1ENR) == 0x30u, "RCC enable layout");
_Static_assert(offsetof(SPI_TypeDef, DR) == 0x0Cu, "SPI DR layout");
_Static_assert(offsetof(I2C_TypeDef, SR1) == 0x14u, "I2C SR1 layout");
_Static_assert(offsetof(ADC_TypeDef, DR) == 0x4Cu, "ADC DR layout");
_Static_assert(offsetof(TIM_TypeDef, ARR) == 0x2Cu, "TIM ARR layout");
_Static_assert(offsetof(DMA_Stream_TypeDef, FCR) == 0x14u, "DMA stream layout");
_Static_assert(GPIOC_BASE == 0x40020800u, "GPIOC base");
_Static_assert(SPI1_BASE == 0x40013000u, "SPI1 base");
_Static_assert(I2C1_BASE == 0x40005400u, "I2C1 base");
_Static_assert(ADC1_BASE == 0x40012000u, "ADC1 base");
_Static_assert(TIM2_BASE == 0x40000000u, "TIM2 base");
_Static_assert(FLASH_END == 0x0807FFFFu, "512 KiB target");
