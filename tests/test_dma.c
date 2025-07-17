#include "../../header/init-stm32f446RE/stm32f4xx.h"
#include "../../header/dma/dma.h"
#include "../../header/common/common.h"


void SysClockConfig(void);
void GPIO_config(void);
void delay(unsigned long time);

int main(void)
{
	SysClockConfig();
  GPIO_config();

	DmaStatusEnum Ret_status =  enable_dma_module(DMA_SELECT_2);
	
  // Setup (already in your GPIO_config)
  RCC->AHB1ENR |= (1 << 0);      // Enable GPIOA clock
  GPIOA->MODER &= ~(3 << (5 * 2)); // Clear mode for PA5
  GPIOA->MODER |=  (1 << (5 * 2)); // Set PA5 to output mode
  GPIOA->OTYPER &= ~(1 << 5);      // Push-pull
  GPIOA->OSPEEDR &= ~(3 << (5 * 2)); // Low speed

  if(Ret_status != DMA_ENABLED)
  {
    while(1)
    {
      GPIOA->ODR |= (1 << 5);       // LED ON
      delay(5000000);
      GPIOA->ODR &= ~(1 << 5);      // LED OFF
      delay(5000000);
    }
  }
  else
  {
    GPIOA->ODR |= (1 << 5);       // LED ON
  }

  while(1);

  /* never goes here */
  return 0;
}

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