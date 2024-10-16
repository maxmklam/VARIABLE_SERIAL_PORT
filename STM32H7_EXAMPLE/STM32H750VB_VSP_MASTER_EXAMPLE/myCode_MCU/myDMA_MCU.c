/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myDMA_MCU.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <string.h> //memcpy()
#include "myDMA_MCU.h"
#include "myDMA_STM32H7.h"



/************** DEFINITIONS ***************************************************/ 

struct _DevDMA gDevDMAtx =
{
	.DMAx = DMA1,
	.circularCnt = 0,
};

struct _DevDMA gDevDMArx =
{
	.DMAx = DMA1,
	.circularCnt = 0,
};



//for test
u64 testTime64stDMAtx;
u64 testTime64EndDMAtx;
u64 testTimeDiff64DMAtx;

u64 testTime64stDMArx;
u64 testTime64EndDMArx;
u64 testTimeDiff64DMArx;



/************** LOCAL FUNCTION PROTOTYPES *************************************/



/************** FUNCTION DEFINITIONS ******************************************/


/*=========================== END OF FILE ====================================*/
