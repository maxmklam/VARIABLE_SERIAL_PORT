/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myDMA_STM32H7.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYDMA_STM32H7_H
#define __MYDMA_STM32H7_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/************** INCLUDES ******************************************************/

#include "myTypedef.h"
#include "dma.h"
#include "myDevice.h"


/************** DEFINITIONS ***************************************************/ 

typedef struct _DevDMA
{
	DMA_TypeDef *DMAx;
	u8 *BuffPt;
	u32 xferLen;
	vu32 circularCnt;
	void (*cbFunc)(enum _DevXferState devXferState, u32 nByteXfered);
	u8 dmaCh;
	volatile enum _DevXferState devXferState;
} _DevDMA;


//for test


/************** EXPORTED FUNCTION *********************************************/

void myDMA_TC_isr(struct _DevDMA *pDevDMA);
u32 myDMA_get_curr_xfered_nbyte(struct _DevDMA *pDevDMA);


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYDMA_STM32H7_H

/*=========================== END OF FILE ====================================*/

