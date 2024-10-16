/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myDevice.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYDEVICE_H
#define __MYDEVICE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"


/************** DEFINITIONS ***************************************************/ 


typedef enum _DevXferState
{
	DEVXFERST_IDLE,
	DEVXFERST_USED,
	DEVXFERST_BUSY,
	
	DEVXFERST_DMATXOK,
	DEVXFERST_DMATXFAIL,
	DEVXFERST_DMARXOK,
	DEVXFERST_DMARXFAIL,
	
	DEVXFERST_SPIOK,
	DEVXFERST_SPIFAIL,
	
	DEVXFERST_TIMCCOK,
	DEVXFERST_TIMCCFAIL,
	DEVXFERST_TIMOUT,
	
	DEVXFERST_VSPOK,
	DEVXFERST_VSPFAIL,
	
}_DevXferState;



/************** EXPORTED FUNCTION *********************************************/




/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYDEVICE_H

/*=========================== END OF FILE ====================================*/
