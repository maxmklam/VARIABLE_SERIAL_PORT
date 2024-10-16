/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTimebase.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "myTimebase.h"

/************** DEFINITIONS ***************************************************/ 


/************** LOCAL FUNCTION PROTOTYPES *************************************/


/************** FUNCTION DEFINITIONS ******************************************/


/*==============================================================================
 @NAME: myTimebase_init()
 @PARAM:
 @RETVAL:
 @DESCRIBE: Provide this function in your MCU dependent platform
==============================================================================*/
__weak void myTimebase_init(void)
{
	#warning !!!YOU NEED TO PROVIDE THE FUNCTION: "myTimebase_init()"
	while(1) ; //Will come to here if MCU not provide this function.
}

/*==============================================================================
 @NAME: myTimebase_tick()
 @PARAM:
 @RETVAL:
 @DESCRIBE: 
==============================================================================*/
__weak u32 myTimebase_tick32(void)
{
	#warning !!!YOU NEED TO PROVIDE THE FUNCTION: "myTimebase_tick32()"
	while(1) ; //Will come to here if MCU not provide this function.
}

__weak u64 myTimebase_tick64(void)
{
	#warning !!!YOU NEED TO PROVIDE THE FUNCTION: "myTimebase_tick64()"
	while(1) ; //Will come to here if MCU not provide this function.
}


/*==============================================================================
 @NAME: myTimebase_diffus()
 @PARAM:
 @RETVAL:
 @DESCRIBE: 
==============================================================================*/
__weak u32 myTimebase_diffus32(u32 Tick)
{
	#warning !!!YOU NEED TO PROVIDE THE FUNCTION: "myTimebase_diffus32()"
	while(1) ; //Will come to here if MCU not provide this function.
}

__weak u64 myTimebase_diffus64(u64 Tick)
{
	#warning !!!YOU NEED TO PROVIDE THE FUNCTION: "myTimebase_diffus64()"
	while(1) ; //Will come to here if MCU not provide this function.
}


/*==============================================================================
 @NAME: myUdelay32()
 @PARAM:
 @RETVAL:
 @DESCRIBE: 
This func require the systick timer LOAD set at it's maximum (2^24 = 16777216), 
free running with interrupt every 16777216 ticks.
==============================================================================*/
void myUdelay32(u32 us)
{
	u32 usChk;
	u32 Tick0;
	u32 Tick1;
	
	Tick0 = myTimebase_tick32();
	while(1)
	{
		Tick1 = myTimebase_tick32();
		usChk = myTimebase_diffus32(Tick1 - Tick0);
		if(usChk >= us)
		{
			break;
		}
	}
}

/*==============================================================================
 @NAME: myUdelay32()
 @PARAM:
 @RETVAL:
 @DESCRIBE: 
This func require the systick timer LOAD set at it's maximum (2^24 = 16777216), 
free running with interrupt every 16777216 ticks.
==============================================================================*/
void myUdelay64(u64 us)
{
	u64 usChk;
	u64 Tick0;
	u64 Tick1;
	
	Tick0 = myTimebase_tick64();
	while(1)
	{
		Tick1 = myTimebase_tick64();
		usChk = myTimebase_diffus64(Tick1 - Tick0);
		if(usChk >= us)
		{
			break;
		}
	}
}





/*==============================================================================
 @NAME: myTimebase32_test()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE: 
==============================================================================*/
void myTimebase32_test(void)
{
	u32 testTime0;
	u32 testTime1;
	u32 testTimeDiff;
	
	
	//For testing time measure (10 sec)
	//-------------------------------
	testTime0 = myTimebase_tick32();
	for(int i=0; i<10; i++) //check clock correctness by observation
	{
		myUdelay32(1000000); //1 sec
		testTime1 = myTimebase_tick32();
		testTimeDiff = myTimebase_diffus32(testTime1 - testTime0);
	}
	
	if(((10000000-100000) > testTimeDiff) || (testTimeDiff > (10000000+100000))) //assume total measure accuracy in 10sec =  +/-100ms
	{
		u8 myTimebaseResult[] = {"myTimebase_test32 FAILED"};
		(void)myTimebaseResult;
		while(1)
		{;}
	}
	else
	{
		u8 myTimebaseResult[] = {"myTimebase_test32 PASSED"};
		(void)myTimebaseResult;
	}
 }


/*==============================================================================
 @NAME: myTimebase64_test()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE: 
==============================================================================*/
void myTimebase64_test(void)
{
	u64 testTime0;
	u64 testTime1;
	u64 testTimeDiff;
	
	
   //For testing time measure (10 sec)
   //-------------------------------
   testTime0 = myTimebase_tick64();
   for(int i=0; i<10; i++) //check clock correctness by observation
	{
		myUdelay64(1000000); //1 sec
		testTime1 = myTimebase_tick64();
		testTimeDiff = myTimebase_diffus64(testTime1 - testTime0);
	}
	
	if(((10000000-100000) > testTimeDiff) || (testTimeDiff > (10000000+100000))) //assume total measure accuracy in 10sec =  +/-100ms
	{
		u8 myTimebaseResult[] = {"myTimebase_test64 FAILED"};
		(void)myTimebaseResult;
		while(1)
		{;}
	}
	else
	{
		u8 myTimebaseResult[] = {"myTimebase_test64 PASSED"};
		(void)myTimebaseResult;
	}
}
 
 
/*=========================== END OF FILE ====================================*/
