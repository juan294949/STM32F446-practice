#include "usart-test.h"
#include "gpio-test.h"
#include "utility.h"
#include "gpio.h"
#include "utility.h"
#include "exti.h"

#include "stm32f4xx_hal_def.h" 
// this includes enables all HALL modules. 
#include "stm32f4xx_hal_spi.h" 
#include "stm32f4xx_hal.h"

#define MIN_DATA 1

void NVIC_PrintStatus(void);
void initialize(void);
int main(void)
{
	HAL_Init();
	/* Initialize Clocks that will be used in the program.*/
	initialize();

  /*Test Usart*/
  // testing only USART2 TX for now. PORTA PIN2 TX and PIN3 RX.
  char StringTest[] = "Testing Uart!\r\n";
  int TestStatus = usart_test_tx(StringTest,sizeof(StringTest)/sizeof(*StringTest));
  TestStatus > 0 ? print("From main-test.c: USART TEST: FAILED\r\n") : print("From main-test.c: USART TEST: SUCCESS\r\n");
	
  /*Test GPIO*/
  // testing on Port A pin 5
  TestStatus = gpio_test();
  TestStatus > 0 ? print("From main-test.c: GPIO TEST: FAILED\r\n") : print("From main-test.c: GPIO TEST: SUCCESS\r\n");

	/*Test SPI With HALL*/
	const char pBufferTX[] = " Testing SPI";
	char pBufferRX[16]={0};
	gpioSelectPinMode(GPIOA, PIN4, ALTFUNC);      // PA2   : Modus = Alt. Funktion
	gpioSelectAltFunc(GPIOA, PIN4, AF5);          // PA2   : AF7 = USART2 Rx
	gpioSelectPinMode(GPIOA, PIN5, ALTFUNC);      // PA3   : Modus = Alt. Funktion
	gpioSelectAltFunc(GPIOA, PIN5, AF5);          // PA3   : AF7 = USART2 Tx
	gpioSelectPinMode(GPIOA, PIN6, ALTFUNC);      // PA3   : Modus = Alt. Funktion
	gpioSelectAltFunc(GPIOA, PIN6, AF5);          // PA3   : AF7 = USART2 Tx
	gpioSelectPinMode(GPIOA, PIN7, ALTFUNC);      // PA3   : Modus = Alt. Funktion
	gpioSelectAltFunc(GPIOA, PIN7, AF5);          // PA3   : AF7 = USART2 Tx
	uint16_t txCounter = 0; 
	HAL_SPI_StateTypeDef StatusVal; 
	uint16_t RetErrorCode;
	SPI_HandleTypeDef SpiCOnfig = { 
												.Instance=SPI1, 
												.Init= 
												{ .Mode=SPI_MODE_MASTER, 
													.Direction=SPI_DIRECTION_2LINES, 
													.DataSize=SPI_DATASIZE_8BIT, 
													.CLKPolarity=SPI_POLARITY_LOW, 
													.CLKPhase=SPI_PHASE_1EDGE, 
													.NSS=SPI_NSS_HARD_OUTPUT, 
													.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_4, 
													.FirstBit=SPI_FIRSTBIT_MSB, 
													.TIMode=SPI_TIMODE_DISABLE, 
													.CRCCalculation=SPI_CRCCALCULATION_DISABLE, 
													.CRCPolynomial=MIN_DATA 
												}, 
												.hdmatx=NULL, 
												.hdmarx=NULL,
											}; 

	/*Initialize the HAL (Hardware abstraction layer*/
	HAL_SPI_Init(&SpiCOnfig);
	__enable_irq();
	TestStatus = (int)HAL_SPI_Transmit(&SpiCOnfig,pBufferTX,(uint16_t)sizeof(pBufferTX)/sizeof(*pBufferTX),1000);
	TestStatus > 0 ? print("From main-test.c: SPI TEST TX: FAILED\r\n") : print("From main-test.c: SPI TEST TX: SUCCESS\r\n");
	HAL_SPI_Receive(&SpiCOnfig,pBufferRX,(uint16_t)sizeof(pBufferRX)/sizeof(*pBufferRX),10000);
	TestStatus = memcmp(pBufferTX,pBufferRX,(size_t)sizeof(pBufferTX)/sizeof(*pBufferTX));
	print("Spected: %s\r\n Received: %s\r\n",pBufferTX,pBufferRX);
	TestStatus != 0 ? print("From main-test.c: SPI TEST RX: FAILED\r\n") : print("From main-test.c: SPI TEST RX: SUCCESS\r\n");

  while(1);

  return 0;
}

void NVIC_PrintStatus(void)
{
    print("\n===== NVIC STATUS =====\r\n");

    for (int i = 0; i < 8; i++)   // STM32F4 has up to 240 IRQs → 8 x 32
    {
        uint32_t enabled  = NVIC->ISER[i];
        uint32_t pending  = NVIC->ISPR[i];
        uint32_t active   = NVIC->IABR[i];

        if (enabled || pending || active)
        {
            print("Block %d:\r\n", i);
            print("  ENABLED : 0x%08lX\r\n", enabled);
            print("  PENDING : 0x%08lX\r\n", pending);
            print("  ACTIVE  : 0x%08lX\r\n", active);
        }
    }
}



void initialize(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

}