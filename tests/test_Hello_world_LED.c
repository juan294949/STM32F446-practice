
/********************************************************************************
* @file    stm32f4xx_hal_conf.h
* @author  
* @brief 
*******************************************************************************/
#include <stdint.h>
#include "../header/init-stm32f446RE/stm32f4xx.h"

void GPIO_config(void)
{

  RCC->AHB1ENR|=(1<<0);
  GPIOA->MODER|=(1<<10);
  GPIOA->OTYPER =0;
  GPIOA->OSPEEDR =0;

}

void delay(unsigned long time)
{
  while(time--);
}
void SysClockConfig(void)
{
 RCC->CR |= 1<<16;  
while (!(RCC->CR & (1<<17)));
RCC->APB1ENR |= 1<<28;
PWR->CR |= 3<<14; 
FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (5<<0);



// AHB PR
RCC->CFGR &= ~(1<<4);
	
// APB1 PR
RCC->CFGR |= (5<<10);
	
// APB2 PR
RCC->CFGR |= (4<<13);

#define PLL_M 	4
#define PLL_N 	180
#define PLL_P 	0  // PLLP = 2

RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (1<<22);

RCC->CR |= (1<<24);
while (!(RCC->CR & (1<<25)));
RCC->CFGR |= (2<<0);
while (!(RCC->CFGR & (2<<2)));

}

int main(void) 
{

  SysClockConfig();
  GPIO_config();

while(1)
{
  GPIOA->BSRRL = (1<<5);
  delay(1000000);
  GPIOA->BSRRL|=((1<<5) <<16);
  delay(1000000);
}

}
