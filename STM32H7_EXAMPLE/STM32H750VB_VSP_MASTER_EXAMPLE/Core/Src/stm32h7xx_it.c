/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "myTypedef.h"
#include "myTimebase.h"
#include "VSP.h"
#include "myDMA_MCU.h"
#include "myDMA_STM32H7.h"
#include "VSP_4_MCU.h"
#include "mySPI_MCU.h"
#include "mySPI_STM32H7.h"
#include "myTIM_MCU.h"
#include "myTIM_STM32H7.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

u32 testTIM1SRreg[256];
u8 testTIM1SRregIndx = 0;

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

#if 0
/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
#endif

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

	
	if(LL_DMA_IsActiveFlag_TC0(DMA1))
	{
		//for test
   	testTime64stDMAtx = myTimebase_tick64();
		
		LL_DMA_ClearFlag_TC0(DMA1);
		//LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0); //Disable DMA Stream

		static u32 dmaCh0IntCnt = 0;
		dmaCh0IntCnt ++;
		
		
		if(gDevDMAtx.dmaCh == LL_DMA_STREAM_0)
		{
			gDevDMAtx.devXferState = DEVXFERST_DMATXOK;
			myDMA_TC_isr(&gDevDMAtx);
		}
 	}
	
	else if(LL_DMA_IsActiveFlag_TE0(DMA1))
	{
		LL_DMA_ClearFlag_TE0(DMA1);
		/* Call Error function */
	}
	
	
  /* USER CODE END DMA1_Stream0_IRQn 0 */

  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */



	if(LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		//for test
   	testTime64stDMAtx = myTimebase_tick64();
		
		LL_DMA_ClearFlag_TC1(DMA1);
		//LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1); //Disable DMA Stream

		static u32 dmaCh1IntCnt = 0;
		dmaCh1IntCnt ++;
		
		
		if(gDevDMAtx.dmaCh == LL_DMA_STREAM_1)
		{
			gDevDMAtx.devXferState = DEVXFERST_DMATXOK;
			myDMA_TC_isr(&gDevDMAtx);
		}
 	}
	
	else if(LL_DMA_IsActiveFlag_TE1(DMA1))
	{
		LL_DMA_ClearFlag_TE1(DMA1);
		/* Call Error function */
	}


/* USER CODE END DMA1_Stream1_IRQn 0 */

  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

	//for debug
	static u32 dmaCh2AllIntCnt = 0;
	dmaCh2AllIntCnt ++;
		
	if(LL_DMA_IsActiveFlag_TC2(DMA1))
	{
		//for test
   	testTime64stDMArx = myTimebase_tick64();
		
		LL_DMA_ClearFlag_TC2(DMA1);
		
		//for debug
		static u32 dmaCh2IntCnt = 0;
		dmaCh2IntCnt ++;
		
		if(gDevDMArx.dmaCh == LL_DMA_STREAM_2)
		{
			gDevDMArx.devXferState = DEVXFERST_DMARXOK;
			myDMA_TC_isr(&gDevDMArx);
		}
	}
	
	else if(LL_DMA_IsActiveFlag_TE2(DMA1))
	{
		//dma error func
	}

  /* USER CODE END DMA1_Stream2_IRQn 0 */

  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

	//for test
	static u32 tim1IntCnt = 0;
	tim1IntCnt ++;
	testTIM1SRreg[testTIM1SRregIndx ++] = TIM1->SR;

	
	//if(LL_TIM_IsActiveFlag_CC1(TIM1) && LL_TIM_IsEnabledIT_CC1(TIM1)) //!!! LL_TIM_IsActiveFlag_CC1() flag checking here will fail because with DMA this flag is cleared by DMA 
	if(LL_TIM_IsEnabledIT_CC1(TIM1))
	{
		//for test
   	testTime64stTIMrx = myTimebase_tick64();
		
		LL_TIM_DisableIT_CC1(TIM1);
		LL_TIM_ClearFlag_CC1(TIM1);
		
		LL_TIM_ClearFlag_CC2(TIM1); //for VSPrx timeout
		LL_TIM_EnableIT_CC2(TIM1);

		//for test
		static u32 tim1ch1IntCnt = 0;
		tim1ch1IntCnt ++;

		if(gDevTIMrx.timCh == LL_TIM_CHANNEL_CH1)
		{
			gDevTIMrx.devXferState = DEVXFERST_TIMCCOK;
			myTIM_CC_isr(&gDevTIMrx);
		}
	}
	//BMC RX timeout timer CC2
	//--------------------------------
	else if(LL_TIM_IsActiveFlag_CC2(TIM1) && LL_TIM_IsEnabledIT_CC2(TIM1))
   {
		//for test
   	testTime64stTIMto = myTimebase_tick64();
		
		LL_TIM_DisableIT_CC2(TIM1);
		LL_TIM_ClearFlag_CC2(TIM1);
	
		LL_TIM_ClearFlag_CC1(TIM1);
		LL_TIM_EnableIT_CC1(TIM1);

		//for test
		static u32 tim1ch2IntCnt = 0;
		tim1ch2IntCnt ++;
		
		if(gDevTIMto.timCh == LL_TIM_CHANNEL_CH2)
		{
			gDevTIMto.devXferState = DEVXFERST_TIMOUT;
			myTIM_CC_isr(&gDevTIMto);
		}
	}
	//--------------------------------

	
	
  /* USER CODE END TIM1_CC_IRQn 0 */
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles SPI2 global interrupt.
  */
void SPI2_IRQHandler(void)
{
  /* USER CODE BEGIN SPI2_IRQn 0 */

#if 0	
	//Check EOT flag value in ISR register
	if(LL_SPI_IsActiveFlag_EOT(SPI2) && LL_SPI_IsEnabledIT_EOT(SPI2))
	{
		//for test
   	testTime64stSPI2 = myTimebase_tick64();
		
		//LL_SPI_DisableIT_EOT(SPI2);
		LL_SPI_ClearFlag_EOT(SPI2);

		//for test
		static u32 SPI2IntCnt = 0;
		SPI2IntCnt ++;
		
		gDevSPIvsp.devXferState = DEVXFERST_SPIOK;
		mySPI_TC_isr(&gDevSPIvsp);
	}
#endif
	
  /* USER CODE END SPI2_IRQn 0 */
  /* USER CODE BEGIN SPI2_IRQn 1 */

  /* USER CODE END SPI2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
