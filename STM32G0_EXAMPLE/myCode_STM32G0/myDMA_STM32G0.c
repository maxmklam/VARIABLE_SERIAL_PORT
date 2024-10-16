/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myDMA_STM32G0.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <string.h> //memcpy()
#include "myDMA_STM32G0.h"
#include "myDMA_MCU.h"


/************** DEFINITIONS ***************************************************/ 


/************** LOCAL FUNCTION PROTOTYPES *************************************/


/************** FUNCTION DEFINITIONS ******************************************/

/*==============================================================================
 @NAME: myDMA_TC_isr()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void myDMA_TC_isr(struct _DevDMA *pDevDMA)
{
	u32 nByteXfered = 0;
	
	
	if(LL_DMA_IsEnabledChannel(pDevDMA->DMAx, pDevDMA->dmaCh) && (LL_DMA_GetMode(pDevDMA->DMAx, pDevDMA->dmaCh) == LL_DMA_MODE_CIRCULAR)) //DMA channel is in circular mode and still enabling, DMA ch will not auto stop but will keep repeating.
	{
		pDevDMA->circularCnt ++;
		nByteXfered = pDevDMA->circularCnt * pDevDMA->xferLen;
	}
	else //DMA channel is in normal mode, DMA ch will auto stop when finished transfer. Or DMA in circular mode but disabled and will trigger an interrupt
	{
		u32 temp = LL_DMA_GetDataLength(pDevDMA->DMAx, pDevDMA->dmaCh);
		
		LL_DMA_DisableChannel(pDevDMA->DMAx, pDevDMA->dmaCh);

		//LL_DMA_SetDataLength(pDevDMA->DMAx, pDevDMA->dmaCh, pDevDMA->xferLen); //reset
		nByteXfered = (pDevDMA->circularCnt +1) * pDevDMA->xferLen - temp; //if DMA normal mode, pDevDMA->circularCnt always = 0
		pDevDMA->circularCnt = 0; //reset
	}
	
	pDevDMA->cbFunc(pDevDMA->devXferState, nByteXfered);	
	
	
}


/*==============================================================================
 @NAME: myDMA_get_curr_xfered_nbyte()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
u32 myDMA_get_curr_xfered_nbyte(struct _DevDMA *pDevDMA)
{
	u32 dmaNDTR;
	u32 dmaNDTR1;
	u32 nbyte;
	vu32 dmaIsrCnt;
	

	dmaNDTR = LL_DMA_GetDataLength(pDevDMA->DMAx, pDevDMA->dmaCh);
	dmaIsrCnt = pDevDMA->circularCnt; //if DMA is in circular mode, dmaIsrCnt increment at every DMA isr
	dmaNDTR1 = LL_DMA_GetDataLength(pDevDMA->DMAx, pDevDMA->dmaCh);

	if(dmaNDTR1 > dmaNDTR) //if second read is larger than first read, means count-down rollover just occurs between 2 successive dma NDTR read, then need to read the dma NDTR again
	{
		dmaIsrCnt = pDevDMA->circularCnt; //read again the upadted(by DMA isr) value
		dmaNDTR = dmaNDTR1;
	}
	
	nbyte = (dmaIsrCnt +1) * pDevDMA->xferLen - dmaNDTR;
	
	return nbyte;
}

/*=========================== END OF FILE ====================================*/
