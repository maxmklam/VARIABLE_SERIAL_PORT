/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTIM_MCU.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <string.h> //memcpy()
#include "myTIM_MCU.h"
#include "myTIM_STM32H7.h"



/************** DEFINITIONS ***************************************************/ 

struct _DevTIM gDevTIMtx =
{
	.TIMx = TIM16,
	.timCh = LL_TIM_CHANNEL_CH1,
	.devXferState = DEVXFERST_IDLE,
};


struct _DevTIM gDevTIMrx =
{
	.TIMx = TIM1,
	.timCh = LL_TIM_CHANNEL_CH1,
	.devXferState = DEVXFERST_IDLE,
};


struct _DevTIM gDevTIMto =
{
	.TIMx = TIM1,
	.timCh = LL_TIM_CHANNEL_CH2,
	.devXferState = DEVXFERST_IDLE,
};


//for test
u64 testTime64stTIMrx;
u64 testTime64EndTIMrx;
u64 testTimeDiff64TIMrx;

u64 testTime64stTIMto;
u64 testTime64EndTIMto;
u64 testTimeDiff64TIMto;


/************** LOCAL FUNCTION PROTOTYPES *************************************/



/************** FUNCTION DEFINITIONS ******************************************/


/*=========================== END OF FILE ====================================*/
