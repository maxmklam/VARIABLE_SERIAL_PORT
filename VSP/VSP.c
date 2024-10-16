/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: VSP.c
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** INCLUDES ******************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "VSP.h"


/************** DEFINITIONS ***************************************************/ 

#if(defined __ICCARM__)
 #include <intrinsics.h>
    
 #define  clz(x)  __CLZ(x)
#endif

//VSP register define (default value)
//------------------------------
u32 VSP_MODEreg = 0; //mode register
u32 VSP_CONTROLreg = 0; //control register
u32 VSP_1TCLKRATEreg = 1000000; //1T-rate register (default 1MHz)
//------------------------------

//for test
//------------------------------
#define  DEBUG0  0
#if(DEBUG0 == 1)
#define  TESTBMCBUFFLEN  3860
u16 testBMCbuff[TESTBMCBUFFLEN];
u32 testBMCbuffIndx = 0;
#endif //#if(DEBUG0 == 1)

#define  DEBUG1  1
#if(DEBUG1 == 1)
#define  TESTVSPDECODEBITFAILBUFFLEN  256
u16 testVSPdecodeBitFailCnt[TESTVSPDECODEBITFAILBUFFLEN];
u32 testVSPdecodeBitFailCntIndx = 0;

u32 testFSindex;
u32 testFEfound;
#endif //#if(DEBUG1 == 1)
//------------------------------


/************** LOCAL FUNCTION PROTOTYPES *************************************/

int VSP_get_numBits(u32 data);
int VSP_pack_bits_to_byte(u32 bitVal, u8 *buff, u32 *byteIndex, u32 *bitIndex);
int VSP_TX_data2bmc_encode_spi(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx);
int VSP_TX_data2bmc_encode_tmr(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx);
int VSP_RX_byte_decode_to_raw(u32 byte, u32 numBits, u8 *rxRawBuff, u32 *rxRawBuffIndex, u32 rxRawBuffSize);
int VSP_RX_bmc2data_decode_timerIc(struct _DevVSPrx *pDevVSPrx, u8 *rxRawBuff, u32 *rxRawLen);

/************** FUNCTION DEFINITIONS ******************************************/
/*==============================================================================
 @NAME: VSP_TX_LL()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak int VSP_TX_LL(struct _DevVSPtx *pDevVSPtx)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_TX_LL()"
	while(1) ; //Will come to here if MCU not provide this function.
}

/*==============================================================================
 @NAME: VSP_RX_LL_get_curr_buff_write_pt()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak u32 VSP_RX_LL_get_curr_buff_write_pt(struct _DevVSPrx *pDevVSPrx)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_RX_LL_get_curr_buff_write_pt()"
	while(1) ; //Will come to here if MCU not provide this function.
}

/*==============================================================================
 @NAME: VSP_RX_LL_init()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak int VSP_RX_LL_init(void)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_RX_LL_init()"
	while(1) ; //Will come to here if MCU not provide this function.
}


/*==============================================================================
 @NAME: VSP_LL_CRC16()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak u16 VSP_LL_CRC16(const u8 *pData, u32 Len8)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_LL_CRC16()"
	while(1) ; //Will come to here if MCU not provide this function.
}

		
/*==============================================================================
 @NAME: VSP_TX_LL_data2bmc_encode()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak int VSP_TX_LL_data2bmc_encode(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_TX_LL_data2bmc_encode()"
	while(1) ; //Will come to here if MCU not provide this function.
}


/*==============================================================================
 @NAME: VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
__weak int VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(u32 lsbByteVal, u32 numBits, struct _DevVSPtx *pDevVSPtx)
{
	#warning !!!YOU NEED TO PROVIDE THE LOW LEVEL FUNCTION: "VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr()"
	while(1) ; //Will come to here if MCU not provide this function.
}




/*==============================================================================
 @NAME: VSP_get_numBits()
 @PARAM: 
 @RETVAL:
 @DESCRIBE: 
==============================================================================*/
int VSP_get_numBits(u32 data)
{
	return (32 - clz(data));
}


/*==============================================================================
 @NAME: VSP_pack_bits_to_byte()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
int VSP_pack_bits_to_byte(u32 bitVal, u8 *buff, u32 *byteIndex, u32 *bitIndex)
{
	int numBits = 0;
	u32 MSB1st = VSP_CONTROLreg & VSP_CR_MSB1ST_MASK;
	u32 msbOrlsbBitShift;
	
	
	//MSB or LSB first
	//-------------------------------
	if(MSB1st)
	{
		msbOrlsbBitShift = 7-(*bitIndex);
	}
	else
	{
		msbOrlsbBitShift = (*bitIndex);
	}
	//-------------------------------

	
	if(bitVal == 1)
	{
		buff[*byteIndex] |= (1<<msbOrlsbBitShift); //set bit=1
	}
	else
	{
		buff[*byteIndex] &= (~(1<<msbOrlsbBitShift)); //set bit=0
	}
	
	(*bitIndex) ++;
	numBits = *bitIndex;
		
	if((*bitIndex) > 7)
	{
		(*bitIndex) = 0;
		(*byteIndex) ++;
	}
	
	return numBits;
}


/*==============================================================================
 @NAME: VSP_TX_data2bmc_encode_spi()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
int VSP_TX_data2bmc_encode_spi(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx)
{
	int i;
	int k;
	u32 numBits;
	u32 rawData;
	u32 preBitval;
	u32 bitIndex = 0;
	u32 numBmcByte = 0;
	
	u32 MSB1st = VSP_CONTROLreg & VSP_CR_MSB1ST_MASK;
	u32 msbOrlsb;
	
	//for info only
	u32 ttlNumBmcPulse = 0;
	u32 ttlNum1T = 0;
	
	
	if(txRawLen == 0)
	{
		return 0;
	}
	
	//add FS (1110111)
	//----------------------------------
	
	//MSB or LSB first
	//-------------------------------
	if(MSB1st)
	{
		msbOrlsb = 0x77; //01110111 --FS, 0,111,0,111 --low 1T, high 3T, low 1T, high 3T
	}
	else
	{
		msbOrlsb = 0xEE; //11101110 --FS, 111,0,111,0 --low 1T, high 3T, low 1T, high 3T
	}
	//-------------------------------
	pDevVSPtx->bmcTxBuffPt[0] = msbOrlsb; 
	
	numBmcByte =1;
	bitIndex = 0;
	preBitval = 1;

	ttlNumBmcPulse = 4; //for info only
	ttlNum1T = 4; //for info only, 0111, or 1110
	
	
	for(i=0; i<txRawLen; i++)
	{	
		//map data to use VSP negative coding: 129 ~ 255 to (-127)~(-1)
		//----------------------------------
		rawData = txRawBuff[i];
		if(rawData > 128)
		{
			rawData = 256 - rawData;
			numBits = VSP_get_numBits(rawData);
			rawData = ~rawData;
		}
		else
		{
			numBits = VSP_get_numBits(rawData);
		}
		//----------------------------------

		//Fill BMC pulse
		//----------------------------------
		for(k=0; k < numBits; k++)
		{
			//MSB or LSB first
			//-------------------------------
			if(MSB1st)
			{
				msbOrlsb = numBits-1-k;
			}
			else
			{
				msbOrlsb = k;
			}
			//-------------------------------
				
			if(rawData & (1<< msbOrlsb)) //start from msb/lsb of data, if data bit = 1
			{				
				if(preBitval == 1) //if previous bit is high
				{
					VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
					VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					
					preBitval = 1;
				}
				else //if previous bit is low
				{
					VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
					
					preBitval = 0;
				}
				
				ttlNumBmcPulse = ttlNumBmcPulse + 2; //for info only,
				ttlNum1T = ttlNum1T +2; //for info only
			}
			else //if(rawData & (1<< msbOrlsb)) //start from msb/lsb of data, if data bit = 0
			{
				if(preBitval == 1) //if previous bit is high
				{
					VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
					VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
					
					preBitval = 0;
				}
				else //if previous bit is low
				{
					VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					
					preBitval = 1;
				}
				
				ttlNumBmcPulse = ttlNumBmcPulse + 1; //for info only,
				ttlNum1T = ttlNum1T +2; //for info only
			}
		} //for(k=0; k < numBits; k++)
		
		
		if(i < (txRawLen -1)) //if not end of data byte, mark NF
		{
			if(preBitval == 1) //if previous bit is high
			{
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
					
				preBitval = 0;
			}
			else //if previous bit is low
			{
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					
				preBitval = 1;
			}
			
			ttlNumBmcPulse = ttlNumBmcPulse +1; //for info only,
			ttlNum1T = ttlNum1T +3; //for info only
		}
		else //if end of data byte, mark FE 1111 or 0000. 11110, 00001 -- additional 0 or 1 at the end of FE mark is to make sure pulse will be captured in timer, when HW VSP is implemented, this is no need
		{
			if(preBitval == 1) //if previous bit is high
			{
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				
				//Make FE a pulse so that it can be captured by timer, no need when HW VSP is implemented. 
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
					
				preBitval = 1;
				
				ttlNumBmcPulse = ttlNumBmcPulse +1; //for info only,
				ttlNum1T = ttlNum1T +4; //for info only
			}
			else //if previous bit is low
			{
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1
				
				//Make FE a pulse so that it can be captured by timer, no need when HW VSP is implemented. 
				VSP_pack_bits_to_byte(0, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=0
				VSP_pack_bits_to_byte(1, pDevVSPtx->bmcTxBuffPt, &numBmcByte, &bitIndex); //set bit=1

				preBitval = 1;
				
				ttlNumBmcPulse = ttlNumBmcPulse +2; //for info only,
				ttlNum1T = ttlNum1T +4; //for info only
			}
		}
				
	} //for(i=0; i<txRawLen; i++)
	
	if(bitIndex > 0) //ceiling last byte
	{
		numBmcByte ++;
	}
	
	pDevVSPtx->numBmcByte = numBmcByte;

	
	return ttlNum1T;
	//return ttlNumBmcPulse;
}
		



/*==============================================================================
 @NAME: VSP_TX_data2bmc_encode_tmr()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
int VSP_TX_data2bmc_encode_tmr(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx)
{
	int i;
	int k;
	u32 numBits;
	u32 rawData;
	u32 preBitval;
	
	u32 MSB1st = VSP_CONTROLreg & VSP_CR_MSB1ST_MASK;
	u32 msbOrlsbBitShift;
	
	//for info only
	u32 ttlNumBmcPulse = 0;
	u32 ttlNum1T = 0;
	
	
	
	if(txRawLen == 0)
	{
		return 0;
	}
	
	pDevVSPtx->numBmcByte = 0;
	

	
	//add FS (1110111)
	//----------------------------------
	
	//MSB or LSB first
	//-------------------------------
	if(MSB1st) //01110111 --FS, 0,111,0,111 --low 1T, high 3T, low 1T, high 3T
	{
		VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x77, 8, pDevVSPtx);
	}
	else //11101110 --FS, 111,0,111,0 --low 1T, high 3T, low 1T, high 3T
	{
		VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0xEE, 8, pDevVSPtx);
	}
	//-------------------------------

	preBitval = 1;
	
	ttlNumBmcPulse = 4; //for info only
	ttlNum1T = 4; //for info only, 0111, or 1110
	
	
	for(i=0; i<txRawLen; i++)
	{	
		//map data to use VSP negative coding: 129 ~ 255 to (-127)~(-1)
		//----------------------------------
		rawData = txRawBuff[i];
		if(rawData > 128)
		{
			rawData = 256 - rawData;
			numBits = VSP_get_numBits(rawData);
			rawData = ~rawData;
		}
		else
		{
			numBits = VSP_get_numBits(rawData);
		}
		//----------------------------------

		//Fill BMC pulse
		//----------------------------------
		for(k=0; k <numBits; k++)
		{
			//MSB or LSB first
			//-------------------------------
			if(MSB1st)
			{
				msbOrlsbBitShift = numBits-1-k;
			}
			else
			{
				msbOrlsbBitShift = k;
			}
			//-------------------------------
				
			if(rawData & (1<< msbOrlsbBitShift)) //start from msb/lsb of data, if data bit = 1
			{				
				if(preBitval == 1) //if previous bit is high
				{
					VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x01, 2, pDevVSPtx); //set bit=0,1
					
					preBitval = 1;
				}
				else //if previous bit is low
				{
					VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x02, 2, pDevVSPtx); //set bit=1,0
					
					preBitval = 0;
				}
				
				ttlNumBmcPulse = ttlNumBmcPulse + 2; //for info only,
				ttlNum1T = ttlNum1T +2; //for info only
			}
			else //if(rawData & (1<< msbOrlsbBitShift)) //start from msb/lsb of data, if data bit = 0
			{
				if(preBitval == 1) //if previous bit is high
				{
					VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x00, 2, pDevVSPtx); //set bit=0,0
										
					preBitval = 0;
				}
				else //if previous bit is low
				{
					VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x03, 2, pDevVSPtx); //set bit=1,1
					
					preBitval = 1;
				}
				
				ttlNumBmcPulse = ttlNumBmcPulse + 1; //for info only,
				ttlNum1T = ttlNum1T +2; //for info only
			}
		} //for(k=0; k < numBits; k++)
		
		
		if(i < (txRawLen -1)) //if not end of data byte, mark NF
		{
			if(preBitval == 1) //if previous bit is high
			{
				VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x00, 3, pDevVSPtx); //set bit=0,0,0
					
				preBitval = 0;
			}
			else //if previous bit is low
			{
				VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x07, 3, pDevVSPtx); //set bit=1,1,1
					
				preBitval = 1;
			}
			
				
			ttlNumBmcPulse = ttlNumBmcPulse +1; //for info only,
			ttlNum1T = ttlNum1T +3; //for info only
		}
		else //if end of data byte, mark FE 1111 or 0000. 11110, 00001 -- additional 0 or 1 at the end of FE mark is to make sure pulse will be captured in timer, when HW VSP is implemented, this is no need
		{
			if(preBitval == 1) //if previous bit is high
			{
				VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x01, 5, pDevVSPtx); //set bit=0,0,0,0,1, 1:Make FE a pulse so that it can be captured by timer, no need when HW VSP is implemented. 
					
				preBitval = 0;
				
				ttlNumBmcPulse = ttlNumBmcPulse +1; //for info only,
			}
			else //if previous bit is low
			{
				//VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x1E, 5, pDevVSPtx); //set bit=1,1,1,1,0, 0:Make FE a pulse so that it can be captured by timer, no need when HW VSP is implemented. 
				VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(0x3D, 6, pDevVSPtx); //set bit=1,1,1,1,0,1, 0:Make FE a pulse so that it can be captured by timer, no need when HW VSP is implemented. 

				preBitval = 1;
				
				ttlNumBmcPulse = ttlNumBmcPulse +2; //for info only,
			}
			
			ttlNum1T = ttlNum1T +4; //for info only
		}
				
	} //for(i=0; i<txRawLen; i++)
	
	return ttlNum1T;
	//return ttlNumBmcPulse;
}
		

/*==============================================================================
 @NAME: VSP_RX_byte_decode_to_raw()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int VSP_RX_byte_decode_to_raw(u32 byte, u32 numBits, u8 *rxRawBuff, u32 *rxRawBuffIndex, u32 rxRawBuffSize)
{
	int retVal = 1;
	u32 chkNumBits;
	u32 VSPmode = VSP_MODEreg & VSP_MODE_MASK;
	u32 MSB1st = VSP_CONTROLreg & VSP_CR_MSB1ST_MASK;

	//MSB or LSB first
	//-------------------------------
	if(MSB1st)
	{
		byte = byte >> (8 - numBits);
	}
	else
	{
		byte &= ((1 << numBits) -1);
	}
	//-------------------------------
	
	chkNumBits = VSP_get_numBits(byte);
	switch(VSPmode)
	{
		case VSP_MODE_NORMAL:
		{
			if(numBits > chkNumBits) //it's zero leading byte
			{
				if((numBits > 7) && (*rxRawBuffIndex == 0)) //first zero leading byte AND 8-bit long is a command byte
				{
					u32 cmd = 1;
				}
				else
				{
					byte = ~byte;
					byte &= ((1 << numBits) -1);
					byte = 256 - byte;
				}
			}
			
			rxRawBuff[*rxRawBuffIndex] = byte;
			(*rxRawBuffIndex) ++;
			if((*rxRawBuffIndex) > rxRawBuffSize)
			{
				(*rxRawBuffIndex) = 0; //if an overflow occurs, just write from beginning of raw data buffer
			}
		}
		break;
		
		case VSP_MODE_DIFF:
		{
		}
		break;
			
		case VSP_MODE_COMPRZ:
		{
		}
		break;
		
		case VSP_MODE_NETWORK:
		{
		}
		break;
		
		default: 
		{
			retVal = -1;
		}
	} //switch(VSPsm)
	
	return retVal;
}
			



/*==============================================================================
 @NAME: VSP_RX_bmc2data_decode_timerIc()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int VSP_RX_bmc2data_decode_timerIc(struct _DevVSPrx *pDevVSPrx, u8 *rxRawBuff, u32 *rxRawLen)
{
	int retVal = -1;
	u32 i;
	u32 indx;
	u32 unReadedNumDat = 0;
	int data = 0;

	int dataM2 = 0;
	int dataM1 = 0;
	volatile enum _VSPsm  VSPsm = VSPSM_FS;
	u32 rxRawBuffIndex = 0;
	
	u32 savedNumBits = 0;
	u8 byte = 0;
	u32 byteIndex = 0;
	u32 bitIndex = 0;
	
	u32 buffWrPt;
	u32 tmrCC1T = pDevVSPrx->tmrCC1T;
	u32 tolerance = pDevVSPrx->tolerance;
	u32 buffRdPt = pDevVSPrx->buffRdPt;
	u32 buffsize = pDevVSPrx->buffsize;
	
	u32 rxRawBuffSize = *rxRawLen;
				

	
	#if(DEBUG1 == 1)
	//for test
	//---------------------------
	static u32 testFuncCallCnt = 0;
	testFuncCallCnt ++;

	static u32 testWait4buffRdPtCnt = 0;
	testWait4buffRdPtCnt = 0;

	testFSindex = 0;
	testFEfound = 0;
	#endif //#if(DEBUG1 == 1)
	//---------------------------
	
	
	
	while(1)
	{
		buffWrPt = VSP_RX_LL_get_curr_buff_write_pt(pDevVSPrx);
				
		if(buffRdPt < buffWrPt)
		{	
			unReadedNumDat = buffWrPt - buffRdPt;
			if(unReadedNumDat > buffsize) //overwritten occurs (data in pDevVSPrx->bmcRxBuffPt haven't process then is overwritten by new received data)
			{
				#if(DEBUG1 == 1)
				//for test
				static u32 bmcDataOverwrittenCnt = 0;
				//bmcDataOverwrittenCnt ++;
				#endif //#if(DEBUG1 == 1)
				
				//overwritten data occurs, and restart from the next un-overwirtten data
				buffRdPt = buffRdPt + unReadedNumDat;
			}
			else
			{
				for(i=0; i<unReadedNumDat; i++) //go through unread data to process
				{				
					//Pulse detection
					//------------------------------------------
					indx = buffRdPt % buffsize;
					
					data = pDevVSPrx->bmcRxBuffPt[indx] +2;
					//data = pDevVSPrx->bmcRxBuffPt[indx];
					
					#if(DEBUG0 == 1)
					//for test
					testBMCbuffIndx = indx % TESTBMCBUFFLEN;
					testBMCbuff[testBMCbuffIndx] = data;
					#endif //#if(DEBUG0 == 1)

									
					for(int k=1; k<=4; k++)
					{
						int Tval = tmrCC1T *k;
						if(abs(data - Tval) <= tolerance) //3: val tolerance
						{
							data = Tval;
							break;
						}
					}

			
					
					//Decode BMC data to raw data
					//-------------------------------------------
					switch(VSPsm)
					{
						case VSPSM_FS: //finding FS mark
						{
							if(buffRdPt >= 2)
							{
								if((dataM2 >= (tmrCC1T *3 -tolerance)) && (dataM1 == tmrCC1T) && (data == (tmrCC1T *3))) //>=3T 1T 3T ---> Found FS
								{
									VSPsm = VSPSM_BIT;
									byteIndex = 0;
									bitIndex = 0;
									rxRawBuffIndex = 0;
									
									#if(DEBUG1 == 1)
									//for test
									testFSindex = indx;
									#endif //#if(DEBUG1 == 1)
								}
							}
						}
						break;
						
						case VSPSM_BIT:
							if(data == tmrCC1T) //half symbol "1"
							{
								VSPsm = VSPSM_HALFBIT1;
							}
							else if(data == tmrCC1T *2) //one symbol "0"
							{
								savedNumBits = VSP_pack_bits_to_byte(0, &byte, &byteIndex, &bitIndex); //one "bit 0"
							}
							else if(data == tmrCC1T *3) //NF found
							{
								//decode received VSP format to raw data according to register setting
								retVal = VSP_RX_byte_decode_to_raw(byte, savedNumBits, rxRawBuff, &rxRawBuffIndex, rxRawBuffSize);
								byteIndex = 0;
								bitIndex = 0;
								byte = 0;
								savedNumBits = 0;
							}
							else if(data == tmrCC1T *4) //FE found
							{

								//Decode completed, decode received VSP format to raw data according to register setting
								retVal = VSP_RX_byte_decode_to_raw(byte, savedNumBits, rxRawBuff, &rxRawBuffIndex, rxRawBuffSize);
								VSPsm = VSPSM_FE_FOUND;
							
								#if(DEBUG1 == 1)
								//for test
								testFEfound = indx;
								static u32 testVSPdecodeOkCnt = 0;
								testVSPdecodeOkCnt ++;
								#endif //#if(DEBUG1 == 1)

								u16 crcRd = *((u16*)&rxRawBuff[rxRawBuffIndex-2]);
								u16 crc16 = VSP_LL_CRC16(rxRawBuff, rxRawBuffIndex-2);
								if(crcRd == crc16)
								{
									#if(DEBUG1 == 1)
									//for test
									static u32 testVSPdecodeCrcOkCnt = 0;
									testVSPdecodeCrcOkCnt ++;
									#endif //#if(DEBUG1 == 1)
									
									pDevVSPrx->devXferState = DEVXFERST_VSPOK;
								}
								else
								{
									#if(DEBUG1 == 1)
									//for test
									static u32 testVSPdecodeCrcFailCnt = 0;
									testVSPdecodeCrcFailCnt ++;
									#endif //#if(DEBUG1 == 1)
									
									pDevVSPrx->devXferState = DEVXFERST_VSPFAIL;
								}
								
								retVal = VSP_RX_LL_init();
								
								return 1;
							}
						break;
							
						case VSPSM_HALFBIT1:
							if(data == tmrCC1T) //another half "1"
							{
								savedNumBits = VSP_pack_bits_to_byte(1, &byte, &byteIndex, &bitIndex); //one "bit 1"
								VSPsm = VSPSM_BIT;
							}
							else if(data == tmrCC1T *3)
							{
								if(dataM2 >= (tmrCC1T *3 -tolerance)) //found FS again
								{
									VSPsm = VSPSM_BIT;
									byteIndex = 0;
									bitIndex = 0;
									rxRawBuffIndex = 0;
									
									#if(DEBUG1 == 1)
									//for test
									testFSindex = indx;
									#endif //#if(DEBUG1 == 1)
								}
							}
							else //error
							{
								#if(DEBUG0 == 1)
								//for test
								testVSPdecodeBitFailCnt[testVSPdecodeBitFailCntIndx] = buffRdPt;
								testBMCbuffIndx = (testBMCbuffIndx +1) % TESTBMCBUFFLEN;
								#endif //#if(DEBUG0 == 1)
									
								VSPsm = VSPSM_FS; //restart from beginning
								//while(1);
							}
						break;
						
						default: 
						{;}
					} //switch(VSPsm)
					//-------------------------------------------
					
					dataM2 = dataM1;
					dataM1 = data;
					buffRdPt = buffRdPt + 1;
				}
			}
		}
		else //buffRdPt == buffWrPt
		{	
			#if(DEBUG1 == 1)
			//for test
			testWait4buffRdPtCnt ++;
			#endif //#if(DEBUG1 == 1)
			
			if(pDevVSPrx->devXferState == DEVXFERST_TIMOUT)
			{
				//read buffWrPt again to make sure really nothing to rx.
				
				buffWrPt = VSP_RX_LL_get_curr_buff_write_pt(pDevVSPrx);
				if(buffRdPt == buffWrPt)
				{
					if(VSPsm != VSPSM_FE_FOUND)
					{
						#if(DEBUG1 == 1)
						//for test
						static u32 testVSPdecodeFailNoFEcnt = 0;
						testVSPdecodeFailNoFEcnt ++;
						#endif //#if(DEBUG1 == 1)
						
						VSPsm = VSPSM_IDLE;
					
						pDevVSPrx->devXferState = DEVXFERST_IDLE;
						VSP_RX_LL_init();
						return -1;
					}
				}
			}
		}
	}
}



/*==============================================================================
 @NAME: VSP_TX()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
int VSP_TX(struct _DevVSPtx *pDevVSPtx, u8 *txRawBuff, u32 txRawLen)
{
	int retVal;
	static u32 ttlNumBmcPulses;

	
	//check and lock VSP dev
	//----------------------
	while(pDevVSPtx->devXferState != DEVXFERST_IDLE) //SPI is currently busy, wait for sometime until it's free.	
	{
		;
	}
	pDevVSPtx->devXferState = DEVXFERST_BUSY;
	//----------------------
	

	ttlNumBmcPulses = VSP_TX_LL_data2bmc_encode(txRawBuff, txRawLen, pDevVSPtx);

	//VSP low level transmit
	//--------------------------------
	retVal = VSP_TX_LL(pDevVSPtx);
	
	return retVal;
}




/*==============================================================================
 @NAME: VSP_RX()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
int VSP_RX(struct _DevVSPrx *pDevVSPrx, u8 *rxRawBuff, u32 *rxRawLen)
{
	int retVal = 0;
	
	
	if(pDevVSPrx->devXferState == DEVXFERST_TIMCCOK)
	{
		retVal = VSP_RX_bmc2data_decode_timerIc(pDevVSPrx, rxRawBuff, rxRawLen); //when received 1st BMC pulse, this func loop forever until FE is received, or timeout
		
		return retVal;
	}

	return retVal;

}






/*=========================== END OF FILE ====================================*/
