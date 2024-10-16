/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTimebase.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYTIMEBASE_H
#define __MYTIMEBASE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/

#include "myTypedef.h" 


/************** DEFINITIONS ***************************************************/ 

/************** EXPORTED FUNCTION *********************************************/
__weak void myTimebase_init(void);

__weak u32 myTimebase_tick32(void);
__weak u64 myTimebase_tick64(void);

__weak u32 myTimebase_diffus32(u32 Tick);
__weak u64 myTimebase_diffus64(u64 Tick);

void myUdelay32(u32 us);
void myUdelay64(u64 us);

void myTimebase32_test(void);
void myTimebase64_test(void);

/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYTIMEBASE_H

/*=========================== END OF FILE ====================================*/

