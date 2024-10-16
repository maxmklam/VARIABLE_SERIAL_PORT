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
#define  SPI2_TX_DMACH    LL_DMA_CHANNEL_1
//#define  SPI2_RX_DMACH    LL_DMA_CHANNEL_3
#define  TMR_TX_DMACH     LL_DMA_CHANNEL_5
#define  TMR_RX_DMACH     LL_DMA_CHANNEL_2
//----------------------------------


extern struct _DevDMA gDevDMAtx;
extern struct _DevDMA gDevDMArx;


//for test
extern u32 testTime32stDMAtx;
extern u32 testTime32EndDMAtx;
extern u32 testTime32DiffDMAtx;

extern u32 testTime32stDMArx;
extern u32 testTime32EndDMArx;
extern u32 testTime32DiffDMArx;


/************** EXPORTED FUNCTION *********************************************/


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYDMA_MCU_H

/*=========================== END OF FILE ====================================*/
