/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: mySPI_STM32G0.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <string.h> //memcpy()
#include "mySPI_STM32G0.h"
#include "mySPI_MCU.h"
#include "myDMA_STM32G0.h"
#include "myDMA_MCU.h"


/************** DEFINITIONS ***************************************************/ 


/************** LOCAL FUNCTION PROTOTYPES *************************************/


/************** FUNCTION DEFINITIONS ******************************************/

/*==============================================================================
 @NAME: SPI_fd_tx_cpu()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int SPI_fd_tx_cpu(struct _DevSPI *pDevSPI)
{
	u32 i;
	
	for(i=0;i<10;i++)
	{
		while(LL_SPI_IsActiveFlag_TXE(pDevSPI->SPIx) != 1U) 
		{;}
		LL_SPI_TransmitData8(pDevSPI->SPIx, pDevSPI->txBuffPt[i]);
	}
	
	return 1;
}


/*==============================================================================
 @NAME: SPI_fd_tx_dma()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int SPI_fd_tx_dma(struct _DevSPI *pDevSPI)
{
	int retVal = 0;
	
	//check and lock DMA dev
	//----------------------
	while(gDevDMAtx.devXferState == DEVXFERST_BUSY)
	{
		;
	}
	gDevDMAtx.devXferState = DEVXFERST_BUSY;

	//check and lock SPI dev
	//----------------------
	while(pDevSPI->devXferState == DEVXFERST_BUSY)
	{
		;
	}
	pDevSPI->devXferState = DEVXFERST_BUSY;
	//----------------------
	
	LL_SPI_Disable(pDevSPI->SPIx);

	LL_DMA_DisableChannel(gDevDMAtx.DMAx, pDevSPI->txDmaCh); //Disable DMA Stream

	//LL_DMA_DisableChannel(gDevDMAtx.DMAx, pDevSPI->txDmaCh); //Disable DMA Stream
	LL_DMA_SetPeriphAddress(gDevDMAtx.DMAx, pDevSPI->txDmaCh, (u32)&pDevSPI->SPIx->DR);
	LL_DMA_SetMemoryAddress(gDevDMAtx.DMAx, pDevSPI->txDmaCh, (u32)pDevSPI->txBuffPt);
	LL_DMA_SetDataLength(gDevDMAtx.DMAx, pDevSPI->txDmaCh, pDevSPI->xferLen);

	LL_SPI_Enable(pDevSPI->SPIx); //Enable SPI
	LL_DMA_EnableChannel(gDevDMAtx.DMAx, pDevSPI->txDmaCh); //Enable DMA Stream
	
	
	return retVal;
}





/*==============================================================================
 @NAME: mySPI_TC_isr()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void mySPI_TC_isr(struct _DevSPI *pDevSPI)
{	
	pDevSPI->cbFunc(pDevSPI->devXferState, pDevSPI->xferLen);
}

/*=========================== END OF FILE ====================================*/
