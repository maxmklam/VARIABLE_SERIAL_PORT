/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "myTypedef.h"
#include "VSP.h"
#include "VSP_4_MCU.h"
#include "mySPI_STM32G0.h"
#include "myTimebase_4_STM32G0.h"
#include "myCRC_STM32G0.h"

#include "myDMA_MCU.h"
#include "myDMA_STM32G0.h"
#include "myTIM_MCU.h"
#include "myTIM_STM32G0.h"

#include "myCompileTime.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define  COMPILETIME  __TIME_UNIX__

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//for test
u32 numVSPtx = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_CRC_Init();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  
	//Time base
	//----------------------
	myTimebase_init();
	//myTimebase64_test();

	//----------------------

	//HW CRC
	//----------------------
	//myHWCRC_test();

	//----------------------


	
 #if(VSP_TX_ENABLE == 1)
	//For VSP transmission
	//----------------------
	static u8 txRawBuff[TXRAWBUFF_SIZE];
	static u32 txRawLen = TXRAWBUFF_SIZE;
	//static u32 numVSPtx = 0;

	u32 i;
	u16 crc16;
	u32 rSeed = COMPILETIME;
	srand(rSeed); //seed for rand()
	//----------------------
 #endif //#if(VSP_TX_ENABLE == 1)

	
 #if(VSP_RX_ENABLE == 1)
	//For VSP reception
	//----------------------
	static u8 rxRawBuff[RXRAWBUFF_SIZE];
	static u32 rxRawLen = RXRAWBUFF_SIZE;
	memset(rxRawBuff, 0, RXRAWBUFF_SIZE);
	int retVal;
	//----------------------
 #endif //#if(VSP_RX_ENABLE == 1)

	

	//NEW SERIAL PORT
	//----------------------
	VSP_LL_init();
	//----------------------

	
	
	/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

 #if(VSP_TX_ENABLE == 1)
	//VSP TX
	//--------------------------------------------------
	memset(txRawBuff, 0, TXRAWBUFF_SIZE);
	
	*((u32*)(&txRawBuff[0])) = numVSPtx;
	for(i=0; i<(TXRAWBUFF_SIZE-4-2); i++)
	{
		//txRawBuff[4+i] = rand() % TXRAWBUFF_SIZE;
		txRawBuff[4+i] = 128;
	}
	
	crc16 = VSP_LL_CRC16(txRawBuff, TXRAWBUFF_SIZE-2);
	*((u16*)(&txRawBuff[TXRAWBUFF_SIZE-2])) = crc16;

	
	VSP_TX(&gDevVSPtx, txRawBuff, txRawLen);
	
	//LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8); //LED on
	myUdelay32(5000); //Note: 300us delay is ok, but has some error packet, 1000us delay is good for receiver to process received data correctly in long run
	//LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8); //LED off
	//myUdelay32(1000); //Note: 300us delay is ok, but has some error packet, 1000us delay is good for receiver to process received data correctly in long run
	//--------------------------------------------------

	//for test
	numVSPtx ++;
	if(numVSPtx == 1000)
	{
		int dummy=0;
	}
 #endif //#if(VSP_TX_ENABLE == 1)
	

 #if(VSP_RX_ENABLE == 1)
	//VSP RX
	//--------------------------------------------------
	//memset(rxRawBuff, 0, RXRAWBUFF_SIZE);
	retVal = VSP_RX(&gDevVSPrx, rxRawBuff, &rxRawLen);
	if(retVal == 1)
	{
		//LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8); //LED on
		//myUdelay32(5000); //Note: 300us delay is ok, but has some error packet, 1000us delay is good for receiver to process received data correctly in long run
	}
	else if(retVal == -1)
	{
		//LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8); //LED off
	}
	//--------------------------------------------------
 #endif //#if(VSP_RX_ENABLE == 1)
	
	
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(64000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
