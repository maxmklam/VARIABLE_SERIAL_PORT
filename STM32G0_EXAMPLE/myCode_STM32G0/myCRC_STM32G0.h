/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: myCRC_STM32G0.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __MYCRC_STM32G0_H
#define __MYCRC_STM32G0_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "crc.h"

	 
/************** DEFINITIONS ***************************************************/    


//------------------------------
#define  CRC8_POLY_9B   0x9B
#define  CRC16_POLY_1021  0x1021
#define  CRC32_POLY_04C11DB7  0x4C11DB7	 

#define  CRC_INITVAL  0xFFFFFFFF
#define  CRC_OUTXOR   0x00000000
	 
#define  CRC_BE  0
#define  CRC_LE  1	 
	 

	 
//For STM32G HW CRC reg setting
//------------------------------
#define  HWCRC_CRC32      LL_CRC_POLYLENGTH_32B //0  //STM32GH7 reg value setting
#define  HWCRC_CRC16      LL_CRC_POLYLENGTH_16B //8  //STM32GH7 reg value setting
#define  HWCRC_CRC8       LL_CRC_POLYLENGTH_8B //16 //STM32GH7 reg value setting

#define  HWCRC_INPUT_NOREFL         LL_CRC_INDATA_REVERSE_NONE //(0)
#define  HWCRC_INPUT_REFL_BYTE      LL_CRC_INDATA_REVERSE_BYTE //(1 <<5)
#define  HWCRC_INPUT_REFL_HALFWORD  LL_CRC_INDATA_REVERSE_HALFWORD //(2 <<5)
#define  HWCRC_INPUT_REFL_WORD      LL_CRC_INDATA_REVERSE_WORD //(3 <<5)
	 
#define  HWCRC_OUTPUT_NOREFL   LL_CRC_OUTDATA_REVERSE_NONE //(0)
#define  HWCRC_OUTPUT_REFL     LL_CRC_OUTDATA_REVERSE_BIT //(1 <<7)
//------------------------------
	 
	 
typedef struct _HWCRCcfg
{
	u32 CR; //CR register value
	u32 poly;
	u32 initVal;
	u8 endian;
	//u8 crcSize;
	//u8 reflIO;
	u8 cont;
} _HWCRCcfg;


/************** EXPORTED FUNCTION *********************************************/

u32 myHWCRC(struct _HWCRCcfg *HWCRCcfg, const u8 *pData, u32 Len8);

u32 myHWCRC32_LE_noRefIO(const u8 *pData, u32 Len8);
u32 myHWCRC32_LE_RefIO(const u8 *pData, u32 Len8);
u32 myHWCRC32_BE_noRefIO(const u8 *pData, u32 Len8);
u32 myHWCRC32_BE_RefIO(const u8 *pData, u32 Len8);

void myHWCRC_test(void);



/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__MYCRC_STM32G0_H

/*=========================== END OF FILE ====================================*/
