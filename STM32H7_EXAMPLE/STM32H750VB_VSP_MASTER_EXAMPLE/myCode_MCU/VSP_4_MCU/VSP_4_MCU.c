/*============= REVISION HISTORY ===============================================
 20241022   maxmklam
 --------   --------------
            - Refined CRC func

 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: VSP_4_MCU.c
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

#include "myDMA_MCU.h"
#include "myDMA_STM32H7.h"
#include "mySPI_MCU.h"
#include "mySPI_STM32H7.h"
#include "myTIM_MCU.h"
#include "myTIM_STM32H7.h"
#include "myCRC_STM32H7.h"

#include "VSP.h"
#include "VSP_4_MCU.h"



/************** DEFINITIONS ***************************************************/ 

u8 bmcTxBuff[BMCTXBUFF_SIZE];
struct _DevVSPtx gDevVSPtx = 
{
	.bmcTxBuffPt = bmcTxBuff,
	.buffsize = BMCTXBUFF_SIZE,
	.cbFunc = VSP_TX_LL_cbFunc,
	.devXferState = DEVXFERST_IDLE,
};


u16 bmcRxBuff[BMCRXBUFF_SIZE];
struct _DevVSPrx gDevVSPrx = 
{
	.bmcRxBuffPt = bmcRxBuff,
	.buffsize = BMCRXBUFF_SIZE,
	.buffWrPt = 0,
	.buffRdPt = 0,
	.cbFunc = VSP_RX_LL_cbFunc,
	.devXferState = DEVXFERST_IDLE,
};




/************** LOCAL FUNCTION PROTOTYPES *************************************/


/************** FUNCTION DEFINITIONS ******************************************/

/*==============================================================================
 @NAME: VSP_RX_LL_config()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void VSP_LL_init(void)
{
	//VSP register config
	//----------------------
	VSP_MODEreg &= (~VSP_MODE_MASK);
	VSP_MODEreg |= VSP_MODE_NORMAL;
	
	VSP_CONTROLreg |= VSP_CR_MSB1ST_MASK; //MSBit first
	//VSP_CONTROLreg &= ~VSP_CR_MSB1ST_MASK; //LSBit first
	
	VSP_1TCLKRATEreg = VSP_1TCLKRATE; //eg, 12000000 (12MHz)
	u32 tmrCC1T = 240000000 / VSP_1TCLKRATEreg; //240000000 is timer clock freq
	
	u32 timrPeriod = 65535;
	//u32 timrPeriod = tmrCC1T * 5;
	if(timrPeriod > 65535)
	{
		timrPeriod = 65535;
	}
	LL_TIM_SetAutoReload(TIM1, timrPeriod);
	LL_TIM_OC_SetCompareCH2(TIM1, tmrCC1T * 10 -1); //Set timeout value for FE
	//LL_TIM_OC_SetCompareCH2(TIM1, tmrCC1T * 5 -1); //Set timeout value for FE //!!! tmrCC1T *5 cause a lot unwanted timeout, so more time like tmrCC1T * 10 can solve the problem
	//LL_TIM_OC_SetCompareCH2(TIM1, 65534); //temporary, CCR2 value long enough to avoid timeout triggered by big pulses before FS

	//----------------------
	
			
 #if(VSP_TX_USED_SPI == 1)
	//Re-setting SPI clock rate according to SPI register SPI_CFG1 MBR bit, and VSP register 1T-rate value
	//----------------------
	//u32 DIV = 240000000 / VSP_1TCLKRATE; //240000000 is SPI clock. //!!! 240000000 MUST be the same frequency for SPI as setting in LL_RCC_PLL2_SetN(240)
	//u32 DIV = 192000000 / VSP_1TCLKRATE; //192000000 is SPI clock. //!!! 192000000 MUST be the same frequency for SPI as setting in LL_RCC_PLL2_SetN(192)
	u32 DIV = 128000000 / VSP_1TCLKRATE; //128000000 is SPI clock. //!!! 128000000 MUST be the same frequency for SPI as setting in LL_RCC_PLL2_SetN(128)
		
	LL_SPI_Disable(gDevSPIvsp.SPIx); //Disable SPI before can change setting
	
	u32 Baudrate = 0;
	while(DIV > 2)
	{
		Baudrate ++;
		DIV = DIV >>1;
	}
	Baudrate = Baudrate << SPI_CFG1_MBR_Pos;
	LL_SPI_SetBaudRatePrescaler(gDevSPIvsp.SPIx, Baudrate);

	//Set SPI MSB/LSB first
	if(VSP_CONTROLreg & VSP_CR_MSB1ST_MASK) //if MSB first
	{
		LL_SPI_SetTransferBitOrder(gDevSPIvsp.SPIx, LL_SPI_MSB_FIRST);
	}
	else
	{
		LL_SPI_SetTransferBitOrder(gDevSPIvsp.SPIx, LL_SPI_LSB_FIRST);
	}
	//----------------------
	
 #else //#if(VSP_TX_USED_TMR == 1)
	LL_TIM_SetAutoReload(gDevTIMtx.TIMx, tmrCC1T -1);
	
 #endif //#if(VSP_TX_USED_SPI == 1)
	
	

  	//Config timer CC value for VSPtx
	//----------------------
	gDevVSPrx.tmrCC1T = tmrCC1T;
	gDevVSPrx.tolerance = 2 + tmrCC1T/10; //3:observation. /100: 1%

	
  	//Config DMA dev struct for VSPtx
	//----------------------
	gDevDMAtx.dmaCh = VSP_TX_DMACH;
	
  	//Config TIM dev struct for VSPtx
	gDevTIMtx.dmaCh = VSP_TX_DMACH;
	
  	//Config SPI dev struct for VSPtx
	gDevSPIvsp.txDmaCh = VSP_TX_DMACH;
	//----------------------
	
	
	
  	//Config TIM dev struct for VSPrx
	//----------------------
	gDevTIMrx.dmaCh = VSP_RX_DMACH;
	gDevTIMrx.buffPt = gDevVSPrx.bmcRxBuffPt;
	gDevTIMrx.xferLen = gDevVSPrx.buffsize;
	gDevTIMrx.cbFunc = gDevVSPrx.cbFunc;
	gDevTIMrx.devXferState = DEVXFERST_BUSY;
	
	gDevTIMto.cbFunc = gDevVSPrx.cbFunc;
	gDevTIMto.devXferState = DEVXFERST_BUSY;

  	//Config DMA dev struct for VSPrx
	gDevDMArx.BuffPt = (u8*)gDevTIMrx.buffPt;
	gDevDMArx.dmaCh = gDevTIMrx.dmaCh;
	gDevDMArx.xferLen = gDevTIMrx.xferLen;
	gDevDMArx.cbFunc = gDevTIMrx.cbFunc;
	gDevDMArx.devXferState = DEVXFERST_BUSY;

	
	
  	//Enable DMA and Timer for VSPrx
	//----------------------
	LL_TIM_ClearFlag_CC1(gDevTIMrx.TIMx);
	LL_TIM_EnableIT_CC1(gDevTIMrx.TIMx);
	
	LL_TIM_DisableIT_CC2(gDevTIMrx.TIMx);
	LL_TIM_ClearFlag_CC2(gDevTIMrx.TIMx);
	
	
	LL_DMA_DisableStream(gDevDMArx.DMAx, gDevDMArx.dmaCh);
	LL_DMA_SetPeriphAddress(gDevDMArx.DMAx, gDevDMArx.dmaCh, (u32)&gDevTIMrx.TIMx->CCR1);
	LL_DMA_SetMemoryAddress(gDevDMArx.DMAx, gDevDMArx.dmaCh, (u32)gDevDMArx.BuffPt);
	LL_DMA_SetDataLength(gDevDMArx.DMAx, gDevDMArx.dmaCh, gDevDMArx.xferLen);
	//----------------------
	
	
  	//Finally enable all hardware
	//----------------------
	LL_SPI_Enable(gDevSPIvsp.SPIx);
	LL_TIM_EnableCounter(gDevTIMrx.TIMx); //VSP rx
	LL_DMA_EnableStream(gDevDMArx.DMAx, gDevDMArx.dmaCh);
}



/*==============================================================================
 @NAME: VSP_RX_LL_init()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void VSP_RX_LL_init(void)
{
	gDevVSPrx.devXferState = DEVXFERST_IDLE;

	LL_DMA_DisableStream(gDevDMArx.DMAx, gDevDMArx.dmaCh); //reset. this will trigger DMA TC interrupt when DMA is currently enabled.
}



/*==============================================================================
 @NAME: VSP_RX_LL_get_curr_buff_write_pt()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
u32 VSP_RX_LL_get_curr_buff_write_pt(struct _DevVSPrx *pDevVSPrx)
{
	return myDMA_get_curr_xfered_nbyte(&gDevDMArx);
}



/*==============================================================================
 @NAME: VSP_TX_LL_data2bmc_encode()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int VSP_TX_LL_data2bmc_encode(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx)
{
	u32 ttlBmcPulses;
	
 #if(VSP_TX_USED_SPI == 1)
	ttlBmcPulses = VSP_TX_data2bmc_encode_spi(txRawBuff, txRawLen, pDevVSPtx);
	
 #else //(VSP_TX_USED_TMR == 1)
	ttlBmcPulses = VSP_TX_data2bmc_encode_tmr(txRawBuff, txRawLen, pDevVSPtx);
	 
 #endif //#if(VSP_TX_USED_SPI == 1)

	return ttlBmcPulses;
}


/*==============================================================================
 @NAME: VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int VSP_TX_LL_data2bmc_encode_fill_bmcByte_tmr(u32 lsbByteVal, u32 numBits, struct _DevVSPtx *pDevVSPtx)
{
	u32 i;
	
	for(i=0; i<numBits; i++)
	{
		if(lsbByteVal & (1<<(numBits-1-i)))
		{
			pDevVSPtx->bmcTxBuffPt[pDevVSPtx->numBmcByte ++] = ONE_1T;
		}
		else
		{
			pDevVSPtx->bmcTxBuffPt[pDevVSPtx->numBmcByte ++] = ZERO_1T;
		}
	}
}


/*==============================================================================
 @NAME: VSP_TX_LL()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
int VSP_TX_LL(struct _DevVSPtx *pDevVSPtx)
{
	int retVal;
	
 #if(VSP_TX_USED_SPI == 1)
	//check and lock SPI dev
	//----------------------
	while(gDevSPIvsp.devXferState != DEVXFERST_IDLE) //SPI is currently busy, wait for sometime until it's free.	
	{
		;
	}
	gDevSPIvsp.devXferState = DEVXFERST_USED;
	//----------------------
	
	//config SPI dev struct
	//----------------------
	gDevSPIvsp.txBuffPt = pDevVSPtx->bmcTxBuffPt;
	gDevSPIvsp.xferLen = pDevVSPtx->numBmcByte;
	gDevSPIvsp.cbFunc = pDevVSPtx->cbFunc;
	//----------------------
	
	//config DMA dev struct
	//----------------------
	gDevDMAtx.xferLen = pDevVSPtx->numBmcByte;
	gDevDMAtx.cbFunc = pDevVSPtx->cbFunc;
	//----------------------

	
	retVal = SPI_fd_tx_dma(&gDevSPIvsp);
	
 #else //#if(VSP_TX_USED_TMR == 1)
	
	//check and lock SPI dev
	//----------------------
	while(gDevTIMtx.devXferState != DEVXFERST_IDLE) //SPI is currently busy, wait for sometime until it's free.	
	{
		;
	}
	gDevTIMtx.devXferState = DEVXFERST_USED;
	//----------------------
	
	//config SPI dev struct
	//----------------------
	gDevTIMtx.buffPt = (u16*)pDevVSPtx->bmcTxBuffPt;
	gDevTIMtx.xferLen = pDevVSPtx->numBmcByte;
	gDevTIMtx.cbFunc = pDevVSPtx->cbFunc;
	//----------------------
	
	//config DMA dev struct
	//----------------------
	gDevDMAtx.xferLen = pDevVSPtx->numBmcByte;
	gDevDMAtx.cbFunc = pDevVSPtx->cbFunc;
	//----------------------

	retVal = TIM_CC_tx_dma(&gDevTIMtx);
	
 #endif //#if(VSP_TX_USED_SPI == 1)
	
	return 1;
}



/*==============================================================================
 @NAME: VSP_TX_LL_cbFunc()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void VSP_TX_LL_cbFunc(enum _DevXferState devXferState, u32 nByteXfered)
{
	//SPI tx is completed, and this cbFunc is called from SPI ISR
	
	switch(devXferState)
	{
		case DEVXFERST_DMATXOK:
		{
			//for test
			testTime64EndDMAtx = myTimebase_tick64();
			
			gDevDMAtx.devXferState = DEVXFERST_IDLE;
			
			//for test
			testTime64stSPI2 = myTimebase_tick64();
			
		 #if(VSP_TX_USED_SPI == 1)
			while(LL_SPI_IsActiveFlag_TXC(gDevSPIvsp.SPIx) != 1)
			{;}
			LL_SPI_ClearFlag_EOT(gDevSPIvsp.SPIx);
			LL_SPI_ClearFlag_TXTF(gDevSPIvsp.SPIx);
			
			gDevSPIvsp.devXferState = DEVXFERST_IDLE;

		 #else //#if(VSP_TX_USED_TMR == 1)
			gDevTIMtx.devXferState = DEVXFERST_IDLE;
		 #endif //#if(VSP_TX_USED_SPI == 1)
			
			//for test
			u64 testTime64SPI2 = myTimebase_tick64();
			testTimeDiff64SPI2 = myTimebase_diffus64(testTime64SPI2 - testTime64stSPI2); //need 9 us when VSP_1TCLKRATE = 12MHz

			gDevVSPtx.devXferState = DEVXFERST_IDLE;
		}
		break;
		
		case DEVXFERST_DMATXFAIL:
		{
		}
		break;
		
		case DEVXFERST_SPIOK:
		{
			//for test
			//testTime64EndSPI2 = myTimebase_tick64();
		}
		break;
		
		case DEVXFERST_SPIFAIL:
		{
		}
		break;
		
		default:
		{;}
	}
}
	

/*==============================================================================
 @NAME: VSP_RX_LL_cbFunc()
 @PARAM:
 @RETVAL:
 @DESCRIBE:
==============================================================================*/
void VSP_RX_LL_cbFunc(enum _DevXferState devXferState, u32 nByteXfered)
{
	switch(devXferState)
	{
		case DEVXFERST_TIMCCOK: //first VSP pulse rxed by Tim1Ch1
		{
			gDevVSPrx.devXferState = DEVXFERST_TIMCCOK;
		}
		break;
		
		case DEVXFERST_TIMCCFAIL:
		{
		}
		break;
		
		case DEVXFERST_TIMOUT: //VSP rx timeout by Tim1Ch2
		{
			gDevVSPrx.devXferState = DEVXFERST_TIMOUT;

			//for test
			testTime64EndTIMto = myTimebase_tick64();
		}
		break;
		
		case DEVXFERST_DMATXOK:
		{
		}
		break;
		
		case DEVXFERST_DMATXFAIL:
		{
		}
		break;
		
		
		case DEVXFERST_DMARXOK:
		{
			//if DMA is circular mode, restart it here
			gDevDMArx.circularCnt = 0; //reset
			LL_DMA_EnableStream(gDevDMArx.DMAx, gDevDMArx.dmaCh);
			
			//for test
			testTime64EndDMArx = myTimebase_tick64();
		}
		break;
		
		case DEVXFERST_DMARXFAIL:
		{
		}
		break;
		
		default:
		{;}
	}
	u32 dummy = 1;
} 



/*==============================================================================
 @NAME: VSP_LL_CRC16()
 @PARAM: 
 @RETVAL: 
 @DESCRIBE:
==============================================================================*/
u16 VSP_LL_CRC16(const u8 *pData, u32 Len8)
{
	u16 crc16;
		
	struct _HWCRCcfg HWCRCcfg = 
	{
		.CRCx = CRC,
		.endian = CRC_LE,
		.poly = CRC16_POLY_1021, //0x1021
		.CRval = HWCRC_CRC16 | HWCRC_INPUT_NOREFL | HWCRC_OUTPUT_NOREFL,
	};
	
	crc16 = myHWCRC(&HWCRCcfg, pData, Len8);
	return crc16;
}	

/*=========================== END OF FILE ====================================*/


