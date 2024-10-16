/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: mySPI_STM32G0.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYSPI_STM32G0_H
#define __MYSPI_STM32G0_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/************** INCLUDES ******************************************************/

#include "myTypedef.h"
#include "spi.h"
#include "myDevice.h"


/************** DEFINITIONS ***************************************************/ 

typedef struct _DevSPI
{
	SPI_TypeDef *SPIx;
	u8 *txBuffPt;
	u8 *rxBuffPt;
	u32 xferLen;
	u8 txDmaCh;
	u8 rxDmaCh;
	void (*cbFunc)(enum _DevXferState devXferState, u32 nByteXfered);
	volatile enum _DevXferState devXferState;
} _DevSPI;


/************** EXPORTED FUNCTION *********************************************/

int SPI_fd_tx_cpu(struct _DevSPI *pDevSPI);
int SPI_fd_tx_dma(struct _DevSPI *pDevSPI);

void mySPI_TC_isr(struct _DevSPI *pDevSPI);

/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYSPI_STM32G0_H

/*=========================== END OF FILE ====================================*/

