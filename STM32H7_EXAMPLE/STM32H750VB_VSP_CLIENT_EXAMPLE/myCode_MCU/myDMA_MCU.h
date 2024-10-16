/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myDMA_MCU.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYDMA_MCU_H
#define __MYDMA_MCU_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "dma.h"

/************** DEFINITIONS ***************************************************/ 

//----------------------------------
#define  SPI2_TX_DMACH    LL_DMA_STREAM_1
//#define  SPI2_RX_DMACH    LL_DMA_STREAM_3
#define  TMR_TX_DMACH     LL_DMA_STREAM_0
#define  TMR_RX_DMACH     LL_DMA_STREAM_2
//----------------------------------


extern struct _DevDMA gDevDMAtx;
extern struct _DevDMA gDevDMArx;


//for test
extern u64 testTime64stDMAtx;
extern u64 testTime64EndDMAtx;
extern u64 testTimeDiff64DMAtx;

extern u64 testTime64stDMArx;
extern u64 testTime64EndDMArx;
extern u64 testTimeDiff64DMArx;


/************** EXPORTED FUNCTION *********************************************/


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYDMA_MCU_H

/*=========================== END OF FILE ====================================*/
