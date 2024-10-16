/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: VSP.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __DevVSP_H
#define __DevVSP_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "myDevice.h"

/************** DEFINITIONS ***************************************************/ 

	 
//VSP register define
//--------------------- 
#define  VSP_MODE_MASK  0x07 //bit[2:0]

#define  VSP_MODE_NORMAL   0
#define  VSP_MODE_DIFF     1
#define  VSP_MODE_COMPRZ   2
#define  VSP_MODE_NETWORK  3

#define  VSP_CR_MSB1ST_MASK  0x01

extern u32 VSP_MODEreg;
extern u32 VSP_CONTROLreg;
extern u32 VSP_1TCLKRATEreg;
//---------------------


typedef struct _DevVSPtx
{
	u8 *bmcTxBuffPt; //bmcBuffPt[BMCBUFF_SIZE]; ring buffer receiving for BMC pulse
	u32 buffsize;
	u32 numBmcByte;
	void (*cbFunc)(enum _DevXferState devXferState, u32 nByteXfered);
	volatile enum _DevXferState devXferState;
}_DevVSPtx;


typedef struct _DevVSPrx
{
	u16 *bmcRxBuffPt; //bmcBuffPt[BMCBUFF_SIZE]; ring buffer receiving for BMC pulse
	u32 buffsize; //PULSEBUFF_SIZE
	u32 buffWrPt; //non-rollover wrpt for ring buff
	u32 buffRdPt; //non-rollover rdpt for ring buff
	u32 tmrCC1T;
	u32 tolerance;
	void (*cbFunc)(enum _DevXferState devXferState, u32 nByteXfered);
	volatile enum _DevXferState devXferState;
}_DevVSPrx;


typedef struct _TimVSPtx
{
	u8 bit1Val;
	u8 bit0Val;
}_TimVSPtx;


//VSP state machine
//--------------------------------------
typedef enum _VSPsm
{
	VSPSM_IDLE,
	VSPSM_FS,
	VSPSM_BIT,
	VSPSM_HALFBIT1,
	VSPSM_FE_FOUND,
} _VSPsm;
//--------------------------------------





   



/************** EXPORTED FUNCTION *********************************************/

int VSP_TX(struct _DevVSPtx *pDevVSPtx, u8 *txRawBuff, u32 txRawLen);
int VSP_RX(struct _DevVSPrx *pDevVSPrx, u8 *rxRawBuff, u32 *rxRawLen);
int VSP_TX_data2bmc_encode_spi(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx);
int VSP_TX_data2bmc_encode_tmr(u8 *txRawBuff, int txRawLen, struct _DevVSPtx *pDevVSPtx);

/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__DevVSP_H

/*=========================== END OF FILE ====================================*/
