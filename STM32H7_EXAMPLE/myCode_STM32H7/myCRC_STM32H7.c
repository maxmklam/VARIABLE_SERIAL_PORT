/*============= REVISION HISTORY ===============================================
 20241022   maxmklam
 --------   --------------
            - Refined CRC func
            - Add HWCRCcfg.cont = 1 example for testing a separated packet.

 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myCRC_STM32H7.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/

#include "myTimebase.h"
#include "myCRC_STM32H7.h"


/************** DEFINITIONS ***************************************************/ 

/************** LOCAL FUNCTION PROTOTYPES *************************************/

/************** FUNCTION DEFINITIONS ******************************************/

/*==============================================================================
 @NAME: myHWCRC()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u32 myHWCRC(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8)
{
	u32 i;
	u32 Len32 = Len8/4;
	u32 Data32;
	

	//check if reset HWCRCcfg->CRCx is needed
	//-------------------------------------
	if(HWCRCcfg->cont == 0) //if HWCRCcfg->cont = 1, the data is continuous packet from last operation, so just keep ALL register retained and go to do HWCRCcfg->CRCx directly.
	{
		if(HWCRCcfg->CRval == LL_CRC_Read_IDR(HWCRCcfg->CRCx)) //if no setting change, then just reset HWCRCcfg->CRCx data reg, no need to program other reg
		{
			HWCRCcfg->CRCx->CR |= CRC_CR_RESET;
		}
		else
		{
			HWCRCcfg->CRCx->CR = CRC_CR_RESET | HWCRCcfg->CRval;
			LL_CRC_SetPolynomialCoef(HWCRCcfg->CRCx, HWCRCcfg->poly);
			LL_CRC_Write_IDR(HWCRCcfg->CRCx, HWCRCcfg->CRval); //Save setting
		}
	}
	//-------------------------------------

	
	for(i=0; i<Len32; i++)
	{
		if(HWCRCcfg->endian == CRC_LE)
		{
			if(HWCRCcfg->CRval & (HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL)) //non-reflect in/out
			{
				Data32 = SWAPBYTE32(((u32*)pData)[i]);
			}
			else
			{
				Data32 = ((u32*)pData)[i];
			}
		}
		
		else //if(HWCRCcfg->endian == CRC_BE)
		{
			if(HWCRCcfg->CRval & (HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL)) //non-reflect in/out
			{
				Data32 = ((u32*)pData)[i];
			}
			else
			{
				Data32 = SWAPBYTE32(((u32*)pData)[i]);
			}
		}
		
		LL_CRC_FeedData32(HWCRCcfg->CRCx, Data32);
	}

	//Last bytes specific handling
	Len8 = Len8 % 4;
	for(int j=0; j<Len8; j++)
	{
		LL_CRC_FeedData8(HWCRCcfg->CRCx, pData[i*4+j]);
	}

		
	return (LL_CRC_ReadData32(HWCRCcfg->CRCx));
}










/*==============================================================================
 @NAME: myHWCRC32_LE_noRefIO()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u32 myHWCRC32_LE_noRefIO(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8)
{
	u32 i;
	u32 Len32 = Len8/4;
	u32 Data32;

	
	HWCRCcfg->CRCx->CR = 0 //Crc config
		| CRC_CR_RESET
		| HWCRC_CRC32
		| HWCRC_OUTPUT_NOREFL
		| HWCRC_INPUT_NOREFL
		;
	LL_CRC_SetPolynomialCoef(HWCRCcfg->CRCx, 0x4C11DB7);
	LL_CRC_SetInitialData(HWCRCcfg->CRCx, 0xFFFFFFFF);

	
	for(i=0; i<Len32; i++)
	{
		Data32 = ((u32*)pData)[i];
		LL_CRC_FeedData32(HWCRCcfg->CRCx, Data32);
	}

	//Last bytes specific handling
	Len8 = Len8 % 4;
	for(int j=0; j<Len8; j++)
	{
		LL_CRC_FeedData8(HWCRCcfg->CRCx, pData[i*4+j]);
	}

		
	return (LL_CRC_ReadData32(HWCRCcfg->CRCx));
}

/*==============================================================================
 @NAME: myHWCRC32_LE_RefIO()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u32 myHWCRC32_LE_RefIO(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8)
{
	u32 i;
	u32 Len32 = Len8/4;
	u32 Data32;

	
	HWCRCcfg->CRCx->CR = 0 //Crc config
		| CRC_CR_RESET
		| HWCRC_CRC32
		| HWCRC_OUTPUT_REFL
		| HWCRC_INPUT_REFL_BYTE
		;
	LL_CRC_SetPolynomialCoef(HWCRCcfg->CRCx, 0x4C11DB7);
	LL_CRC_SetInitialData(HWCRCcfg->CRCx, 0xFFFFFFFF);
	//LL_CRC_SetInputDataReverseMode(HWCRCcfg->CRCx, LL_CRC_INDATA_REVERSE_BYTE);
	//LL_CRC_SetOutputDataReverseMode(HWCRCcfg->CRCx, LL_CRC_OUTDATA_REVERSE_BIT);

	for(i=0; i<Len32; i++)
	{
		Data32 = ((u32*)pData)[i];
		LL_CRC_FeedData32(HWCRCcfg->CRCx, Data32);
	}

	//Last bytes specific handling
	Len8 = Len8 % 4;
	for(int j=0; j<Len8; j++)
	{
		LL_CRC_FeedData8(HWCRCcfg->CRCx, pData[i*4+j]);
	}

		
	return (LL_CRC_ReadData32(HWCRCcfg->CRCx));
}


/*==============================================================================
 @NAME: myHWCRC32_BE_noRefIO()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u32 myHWCRC32_BE_noRefIO(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8)
{
	u32 i;
	u32 Len32 = Len8/4;
	u32 Data32;

	
	HWCRCcfg->CRCx->CR = 0 //Crc config
		| CRC_CR_RESET
		| HWCRC_CRC32
		| HWCRC_OUTPUT_NOREFL
		| HWCRC_INPUT_NOREFL
		;
	LL_CRC_SetPolynomialCoef(HWCRCcfg->CRCx, 0x4C11DB7);
	LL_CRC_SetInitialData(HWCRCcfg->CRCx, 0xFFFFFFFF);

	
	for(i=0; i<Len32; i++)
	{
		Data32 = SWAPBYTE32(((u32*)pData)[i]);
		LL_CRC_FeedData32(HWCRCcfg->CRCx, Data32);
	}

	//Last bytes specific handling
	Len8 = Len8 % 4;
	for(int j=0; j<Len8; j++)
	{
		LL_CRC_FeedData8(HWCRCcfg->CRCx, pData[i*4+j]);
	}

		
	return (LL_CRC_ReadData32(HWCRCcfg->CRCx));
}

/*==============================================================================
 @NAME: myHWCRC32_BE_RefIO()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u32 myHWCRC32_BE_RefIO(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8)
{
	u32 i;
	u32 Len32 = Len8/4;
	u32 Data32;

	
	HWCRCcfg->CRCx->CR = 0 //Crc config
		| CRC_CR_RESET
		| HWCRC_CRC32
		| HWCRC_OUTPUT_REFL
		| HWCRC_INPUT_REFL_BYTE
		;
	LL_CRC_SetPolynomialCoef(HWCRCcfg->CRCx, 0x4C11DB7);
	LL_CRC_SetInitialData(HWCRCcfg->CRCx, 0xFFFFFFFF);
	//LL_CRC_SetInputDataReverseMode(HWCRCcfg->CRCx, LL_CRC_INDATA_REVERSE_BYTE);
	//LL_CRC_SetOutputDataReverseMode(HWCRCcfg->CRCx, LL_CRC_OUTDATA_REVERSE_BIT);

	for(i=0; i<Len32; i++)
	{
		Data32 = SWAPBYTE32(((u32*)pData)[i]);
		LL_CRC_FeedData32(HWCRCcfg->CRCx, Data32);
	}

	//Last bytes specific handling
	Len8 = Len8 % 4;
	for(int j=0; j<Len8; j++)
	{
		LL_CRC_FeedData8(HWCRCcfg->CRCx, pData[i*4+j]);
	}

		
	return (LL_CRC_ReadData32(HWCRCcfg->CRCx));
}












/*==============================================================================
 @NAME: myHWCRC_test()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:

u8 DataTest[39]={0x21, 0x10, 0x00, 0x00, 0x63, 0x30, 0x42, 0x20, 0xa5, 0x50, 0x84, 0x40, 0xe7, 0x70, 0xc6, 0x60, 0x4a, 0xa1, 0x29, 0x91, 0x8c, 0xc1, 0x6b, 0xb1, 0xce, 0xe1, 0xad, 0xd1, 0x31, 0x12, 0xef, 0xf1, 0x52, 0x22, 0x73, 0x32, 0xa1, 0xb2, 0xc3};

CRC8
--------------
poly: 0x9B
Init: 0xFF
OutXor: 0x00
RefIn/RefOut: NO/NO, Expected Result: 0x0c
RefIn/RefOut: YES/YES, Expected Result: 0xaf

CRC16
--------------
poly: 0x1021
Init: 0xFFFF
OutXor: 0x0000
Little Endian, RefIn/RefOut: NO/NO, Expected Result: 0xb553f395
Little Endian, RefIn/RefOut: YES/YES, Expected Result: 0xa3ba7e3f
Bit Endian, RefIn/RefOut: NO/NO, Expected Result: 0xb553f395
Bit Endian, RefIn/RefOut: YES/YES, Expected Result: 0xa3ba7e3f

CRC32
--------------
poly: 0x04C11DB7
Init: 0xFFFFFFFF
OutXor: 0x00000000
Little Endian, RefIn/RefOut: NO/NO, Expected Result: 0xb553f395
Little Endian, RefIn/RefOut: YES/YES, Expected Result: 0xa3ba7e3f
Bit Endian, RefIn/RefOut: NO/NO, Expected Result: 0xb553f395
Bit Endian, RefIn/RefOut: YES/YES, Expected Result: 0xa3ba7e3f
==============================================================================*/
#define  DATALEN  39
u8 DataTest[DATALEN]={
	0x21, 0x10, 0x00, 0x00, 0x63, 0x30, 0x42, 0x20, 0xa5, 0x50, 0x84, 0x40, 0xe7, 0x70, 0xc6, 0x60, 
	0x4a, 0xa1, 0x29, 0x91, 0x8c, 0xc1, 0x6b, 0xb1, 0xce, 0xe1, 0xad, 0xd1, 0x31, 0x12, 0xef, 0xf1, 
	0x52, 0x22, 0x73, 0x32, 0xa1, 0xb2, 0xc3};

u32 Crc8res_Refl = 0xaf;
u32 Crc8res_noRefl = 0x0c;

u32 Crc16res_LE_noRefl = 0xd926;
u32 Crc16res_LE_Refl = 0x4926;

u32 Crc16res_BE_noRefl = 0x21af;
u32 Crc16res_BE_Refl = 0xbf4f;

u32 Crc32res_LE_noRefl = 0xd4df997a;
u32 Crc32res_LE_Refl = 0x0fb39fbb;

u32 Crc32res_BE_noRefl = 0xb553f395;
u32 Crc32res_BE_Refl = 0xa3ba7e3f;


void myHWCRC_test(void)
{
	u8 Crc8cal;
	u16 Crc16cal;
	u32 Crc32cal;
	
	struct _HWCRCcfg HWCRCcfg = {.CRCx = CRC};
	
	//For testing time measure
	//-------------------------------
	u64 CrcTimTest0;
	u64 CrcTimTest1;
	u64 CrcTimTestDiffUs;
	//-------------------------------
	

	//CRC8 test
	
	//CRC8 test,  NON in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();

	HWCRCcfg.cont = 0;
	HWCRCcfg.poly = CRC8_POLY_9B; //0x9B
	HWCRCcfg.CRval = HWCRC_CRC8 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL;
	Crc8cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc8cal != Crc8res_noRefl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}

	//CRC8 test, in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.poly = CRC8_POLY_9B; //0x9B
	HWCRCcfg.CRval = HWCRC_CRC8 | HWCRC_INPUT_REFL_BYTE | HWCRC_OUTPUT_REFL;
	Crc8cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc8cal != Crc8res_Refl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}

	


	
	//CRC16 test
	
	//CRC16 test,  LITTLE ENDIAN, NON in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_LE;
	HWCRCcfg.poly = CRC16_POLY_1021; //0x1021
	HWCRCcfg.CRval = HWCRC_CRC16 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL;
	Crc16cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc16cal != Crc16res_LE_noRefl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	
	//CRC16 test,  LITTLE ENDIAN, in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_LE;
	HWCRCcfg.poly = CRC16_POLY_1021; //0x1021
	HWCRCcfg.CRval = HWCRC_CRC16 | HWCRC_INPUT_REFL_BYTE | HWCRC_OUTPUT_REFL;
	Crc16cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc16cal != Crc16res_LE_Refl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	
	//CRC16 test,  BIG ENDIAN, NON in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_BE;
	HWCRCcfg.poly = CRC16_POLY_1021; //0x1021
	HWCRCcfg.CRval = HWCRC_CRC16 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL;
	Crc16cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc16cal != Crc16res_BE_noRefl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	
	//CRC16 test,  BIG ENDIAN, in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_BE;
	HWCRCcfg.poly = CRC16_POLY_1021; //0x1021
	HWCRCcfg.CRval = HWCRC_CRC16 | HWCRC_INPUT_REFL_BYTE | HWCRC_OUTPUT_REFL;
	Crc16cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc16cal != Crc16res_BE_Refl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	

	

	
	//CRC32 test
	
	//CRC32 test,  LITTLE ENDIAN, NON in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_LE;
	HWCRCcfg.poly = CRC32_POLY_04C11DB7; //0x4C11DB7
	HWCRCcfg.CRval = HWCRC_CRC32 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL;
	Crc32cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc32cal != Crc32res_LE_noRefl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	
	//CRC32 test,  LITTLE ENDIAN, in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_LE;
	HWCRCcfg.poly = CRC32_POLY_04C11DB7; //0x4C11DB7
	HWCRCcfg.CRval = HWCRC_CRC32 | HWCRC_INPUT_REFL_BYTE | HWCRC_OUTPUT_REFL;
	Crc32cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc32cal != Crc32res_LE_Refl) //if not correct
	{
		while(1) ;
		//int dummy = 1;
	}
	

	//CRC32 test,  BIG ENDIAN, NON in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_BE;
	HWCRCcfg.poly = CRC32_POLY_04C11DB7; //0x4C11DB7
	HWCRCcfg.CRval = HWCRC_CRC32 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL;
	Crc32cal = myHWCRC(&HWCRCcfg, DataTest, DATALEN);
	
	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc32cal != Crc32res_BE_noRefl) //if not correct
	{
		while(1) ;
		////int dummy = 1;
	}
	
	//CRC32 test,  BIG ENDIAN, in/out Reflect
	//-------------------------------------------------------------------
	CrcTimTest0 = myTimebase_tick64();
	
	HWCRCcfg.cont = 0;
	HWCRCcfg.endian = CRC_BE;
	HWCRCcfg.poly = CRC32_POLY_04C11DB7; //0x4C11DB7
	HWCRCcfg.CRval = HWCRC_CRC32 | HWCRC_INPUT_REFL_BYTE | HWCRC_OUTPUT_REFL;
	Crc32cal = myHWCRC(&HWCRCcfg, DataTest, 16); //separate DATALEN to two parts for testing the HWCRCcfg.cont = 1.

	HWCRCcfg.cont = 1; //1: continue from last operation
	Crc32cal = myHWCRC(&HWCRCcfg, &DataTest[16], DATALEN -16); //separate DATALEN to two parts for testing the HWCRCcfg.cont = 1.

	//For testing time measure
	//-------------------------------
	CrcTimTest1 = myTimebase_tick64();
 	CrcTimTestDiffUs = myTimebase_diffus64(CrcTimTest1 - CrcTimTest0);
	//-------------------------------
	if(Crc32cal != Crc32res_BE_Refl) //if not correct
	{
		while(1) ;
		////int dummy = 1;
	}
	
	

	
	//CRC32 function with fixed config
	//-------------------------------------------------------------------
	Crc32cal = myHWCRC32_LE_noRefIO(&HWCRCcfg, DataTest, DATALEN); //need data swap
	Crc32cal = myHWCRC32_LE_RefIO(&HWCRCcfg, DataTest, DATALEN); //need data swap
	Crc32cal = myHWCRC32_BE_noRefIO(&HWCRCcfg, DataTest, DATALEN); //no need data swap
	Crc32cal = myHWCRC32_BE_RefIO(&HWCRCcfg, DataTest, DATALEN); //no need data swap
	
}



/*=========================== END OF FILE ====================================*/
