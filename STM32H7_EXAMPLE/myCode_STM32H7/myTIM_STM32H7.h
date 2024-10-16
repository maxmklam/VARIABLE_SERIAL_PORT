/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTIM_STM32H7.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYTIM_STM32H7_H
#define __MYTIM_STM32H7_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/************** INCLUDES ******************************************************/

#include "myTypedef.h"
#include "tim.h"
#include "myDevice.h"


/************** DEFINITIONS ***************************************************/ 

#define  ZERO_1T  0x48 //timer reg CCMR value for force inactive output of Timer CC pin
#define  ONE_1T  0x58 //timer reg CCMR value for force active output of Timer CC pin
	
 
typedef struct _DevTIM
{
	TIM_TypeDef *TIMx;
	u16 *buffPt;
	u32 xferLen;
	void (*cbFunc)(enum _DevXferState devXferState, u32 nByteXfered);
	u8 timCh;
	u8 dmaCh;
	volatile enum _DevXferState devXferState;
} _DevTIM;


/************** EXPORTED FUNCTION *********************************************/

int TIM_CC_tx_cpu(TIM_TypeDef *TIMx, u8 *txBuff, u32 Len);
int TIM_CC_tx_dma(struct _DevTIM *pDevTIM);

void myTIM_CC_isr(struct _DevTIM *pDevTIM);


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYTIM_STM32H7_H

/*=========================== END OF FILE ====================================*/

