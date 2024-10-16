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
#include "myDMA_STM32G0.h"



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
u32 testTime32stDMAtx;
u32 testTime32EndDMAtx;
u32 testTime32DiffDMAtx;

u32 testTime32stDMArx;
u32 testTime32EndDMArx;
u32 testTime32DiffDMArx;



/************** LOCAL FUNCTION PROTOTYPES *************************************/



/************** FUNCTION DEFINITIONS ******************************************/


/*=========================== END OF FILE ====================================*/
