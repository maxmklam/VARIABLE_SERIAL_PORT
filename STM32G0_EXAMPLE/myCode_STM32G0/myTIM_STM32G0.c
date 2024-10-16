/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTIM_STM32G0.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <string.h> //memcpy()
#include "myTIM_STM32G0.h"
#include "myTIM_MCU.h"
#include "myDMA_STM32G0.h"
#include "myDMA_MCU.h"


/************** DEFINITIONS ***************************************************/ 


/************** LOCAL FUNCTION PROTOTYPES *************************************/


/************** FUNCTION DEFINITIONS ******************************************/

/*==============================================================================
 @NAME: TIM_CC_tx_dma()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int TIM_CC_tx_dma(struct _DevTIM *pDevTIM)
{
	int retVal = 0;
	
	//check and lock DMA dev
	//----------------------
	while(gDevDMAtx.devXferState == DEVXFERST_BUSY)
	{
		;
	}
	gDevDMAtx.devXferState = DEVXFERST_BUSY;

	//check and lock TIM dev
	//----------------------
	while(pDevTIM->devXferState == DEVXFERST_BUSY)
	{
		;
	}
	pDevTIM->devXferState = DEVXFERST_BUSY;
	//----------------------
	
	LL_TIM_DisableCounter(pDevTIM->TIMx);

	LL_DMA_DisableChannel(gDevDMAtx.DMAx, pDevTIM->dmaCh); //Disable DMA Stream

	LL_DMA_SetPeriphAddress(gDevDMAtx.DMAx, pDevTIM->dmaCh, (u32)&pDevTIM->TIMx->CCMR1);
	LL_DMA_SetMemoryAddress(gDevDMAtx.DMAx, pDevTIM->dmaCh, (u32)pDevTIM->buffPt);
	LL_DMA_SetDataLength(gDevDMAtx.DMAx, pDevTIM->dmaCh, pDevTIM->xferLen);
	
	LL_TIM_DisableDMAReq_UPDATE(pDevTIM->TIMx);

	LL_DMA_EnableChannel(gDevDMAtx.DMAx, pDevTIM->dmaCh);
	
	LL_TIM_EnableDMAReq_UPDATE(pDevTIM->TIMx);

	LL_TIM_EnableCounter(pDevTIM->TIMx);
	
	return retVal;
}


/*==============================================================================
 @NAME: myTIM_CC_isr()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void myTIM_CC_isr(struct _DevTIM *pDevTIM)
{
	pDevTIM->cbFunc(pDevTIM->devXferState, pDevTIM->xferLen);
}


/*=========================== END OF FILE ====================================*/
