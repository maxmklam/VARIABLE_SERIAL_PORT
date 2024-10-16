/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTim_MCU.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYTIM_MCU_H
#define __MYTIM_MCU_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "tim.h"


/************** DEFINITIONS ***************************************************/ 

#define  VSP_TX_TIMER     TIM16 
#define  VSP_RX_TIMER     TIM1 

extern struct _DevTIM gDevTIMtx;
extern struct _DevTIM gDevTIMrx;
extern struct _DevTIM gDevTIMto;



//for test
extern u32 testTime32stTIMrx;
extern u32 testTime32EndTIMrx;
extern u32 testTimeDiff64TIMrx;

extern u32 testTime32stTIMto;
extern u32 testTime32EndTIMto;
extern u32 testTimeDiff64TIMto;


/************** EXPORTED FUNCTION *********************************************/



/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYTIM_MCU_H

/*=========================== END OF FILE ====================================*/
