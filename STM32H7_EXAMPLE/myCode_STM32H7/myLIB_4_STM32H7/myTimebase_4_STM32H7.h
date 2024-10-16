/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTimebase_4_STM32H7.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYTIMEBASE_4_STM32H7_H
#define __MYTIMEBASE_4_STM32H7_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "tim.h"
#include "myTimebase.h"

/************** DEFINITIONS ***************************************************/ 

#define  TIMEBASE_64BIT  1 //use 64bit for timebase for supporting higher MCU freq OR for myUdelay() to support long time measurement

extern vu32 gTimebaseIntCnt;


/************** EXPORTED FUNCTION *********************************************/


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYTIMEBASE_4_STM32H7_H

/*=========================== END OF FILE ====================================*/

