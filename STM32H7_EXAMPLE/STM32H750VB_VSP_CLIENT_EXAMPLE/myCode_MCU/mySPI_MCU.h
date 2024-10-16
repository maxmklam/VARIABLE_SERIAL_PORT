/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: mySPI_MCU.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYSPI_MCU_H
#define __MYSPI_MCU_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "spi.h"

	 
/************** DEFINITIONS ***************************************************/ 

#define  SPI2_BUFF_SIZE  1024
	 
extern struct _DevSPI gDevSPIvsp;


//for test
extern u64 testTime64stSPI2;
extern u64 testTime64EndSPI2;
extern u64 testTimeDiff64SPI2;



/************** EXPORTED FUNCTION *********************************************/


/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYSPI_MCU_H

/*=========================== END OF FILE ====================================*/
