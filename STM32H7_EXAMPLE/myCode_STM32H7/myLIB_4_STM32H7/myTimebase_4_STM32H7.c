/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myTimebase_4_STM32H7.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "myTimebase_4_STM32H7.h"

/************** DEFINITIONS ***************************************************/ 

#define  SYSTICK_CLK_DIV8  0 //STM32 systick support div/8


#if(SYSTICK_CLK_DIV8 == 1)
  #define  TIMEBASE_CLK  (SystemCoreClock /8) //Systick timer Clock = Sysclk/8 = 480000000/8 = 60000000 (by clear the SysTick_CTRL_TICKINT_Msk bit in SysTick->CTRL)
#else
  #define  TIMEBASE_CLK  (SystemCoreClock) //Systick timer Clock = Sysclk = 480000000
#endif

#define  TIMEBASE_LOAD  16777215 //max period of Systick timer: 2^24=16777216
  
#define  myTimebase_curCnt()  (SysTick->VAL)

vu32 gTimebaseIntCnt = 0; //Systick interrupt count, period=2^24

  

/************** LOCAL FUNCTION PROTOTYPES *************************************/

/************** FUNCTION DEFINITIONS ******************************************/
/*==============================================================================
 @NAME: SysTick_Handler()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void SysTick_Handler(void) //You need to comment out this function in stm32h7xx_it.c file
{
	gTimebaseIntCnt ++;
}


/*==============================================================================
 @NAME: myTimebase_init()
 @PARAM:
 @RETVAL:
 @DESCRIBE:Note!! This func is used to replace the __weak func in SOFTWARE/myLIB/myTimebase.c
==============================================================================*/
void myTimebase_init(void)
{
	SysTick->LOAD  = TIMEBASE_LOAD; //16777215 (0xFF FFFF), give out a interrupt time: 16777216 / 60Mhz = 0.2796 sec
	NVIC_SetPriority(SysTick_IRQn, 0);

	SysTick->VAL = 0UL; //Load the SysTick Counter Value
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk
					#if(SYSTICK_CLK_DIV8 != 1)
		           | SysTick_CTRL_CLKSOURCE_Msk //SysTick_CTRL_CLKSOURCE_Msk: 1: SYSCLK, 0: SYSCLK/8 = 480000000 MHz /8 = 60MHz
					#endif
		           | SysTick_CTRL_ENABLE_Msk; //Enable SysTick IRQ and SysTick Timer
}



/*==============================================================================
 @NAME: myTimebase_tick() OR myTimebase_tick64()
 @PARAM:
 @RETVAL:
 @DESCRIBE:Note!! This func is used to replace the __weak func in SOFTWARE/myLIB/myTimebase.c
==============================================================================*/
#if(TIMEBASE_64BIT == 1)
u64 myTimebase_tick64(void)
{
	//This func require the systick timer LOAD set at it's maximum (2^24 = 16777216), 
	//free running with interrupt every 16777216 ticks.
	
	u64 Tick;
	u64 IntCnt;
	u64 Tick1;

#else
u32 myTimebase_tick32(void)
{
	//This func require the systick timer LOAD set at it's maximum (2^24 = 16777216), 
	//free running with interrupt every 16777216 ticks.
	
	u32 Tick;
	u32 IntCnt;
	u32 Tick1;
#endif
	
	Tick = myTimebase_curCnt(); //first read of timer tick
	IntCnt = gTimebaseIntCnt;
	Tick1 = myTimebase_curCnt(); //second read of timer tick

	if(Tick1 > Tick) //if second read is larger than first read, means count-down rollover just occurs between 2 successive tick read, then need to read again the timer interrupt count
	{
		IntCnt = gTimebaseIntCnt; //read again for just upadted(by myTimebase timer isr) latest value
		Tick = Tick1;
	}
	
	
	return ((IntCnt << 24) + (TIMEBASE_LOAD+1 - Tick)); //higher 24bit contain gTimebaseIntCnt, lower 24bit contains current systick timer tick.	
}




/*==============================================================================
 @NAME: myTimebase_diffus() OR myTimebase_diffus64()
 @PARAM:
 @RETVAL:
 @DESCRIBE: 
==============================================================================*/
#if(TIMEBASE_64BIT == 1)
u64 myTimebase_diffus64(u64 Tick)
{
	return (Tick / (u64)(TIMEBASE_CLK/1000000));
}

#else
u32 myTimebase_diffus32(u32 Tick)
{
	return (Tick / (u32)(TIMEBASE_CLK/1000000));
}
#endif



/*=========================== END OF FILE ====================================*/
