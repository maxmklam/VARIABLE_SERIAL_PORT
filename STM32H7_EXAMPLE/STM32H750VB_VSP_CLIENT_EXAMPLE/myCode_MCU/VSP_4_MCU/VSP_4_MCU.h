/*============= REVISION HISTORY ===============================================
 20231026   maxmklam
 --------   --------------
            File Creation

==============================================================================*/
/*==============================================================================
 @FILE: VSP_4_MCU.h
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
 ==============================================================================*/

/************** HEADER ********************************************************/
#ifndef __VSP_4_MCU_H
#define __VSP_4_MCU_H

#ifdef __cplusplus
 extern "C" {
#endif 

/************** INCLUDES ******************************************************/
#include "myTypedef.h"
#include "VSP.h"


/************** DEFINITIONS ***************************************************/ 

#define  VSP_TX_ENABLE  0
#define  VSP_RX_ENABLE  1

#define  VSP_TX_USED_SPI  1 //0: VSP_TX_USED_TMR
	 
//----------------------------------------------
#ifndef VSP_TX_USED_SPI
 #error "VSP_TX_USED_SPI is not defined!!!
#endif

#if(VSP_TX_USED_SPI == 1)
 #define  VSP_TX_DMACH     SPI2_TX_DMACH
#else //(VSP_TX_USED_TMR == 1)
 #define  VSP_TX_DMACH     TMR_TX_DMACH	
#endif //#if(VSP_TX_USED_SPI == 1)

#define  VSP_RX_DMACH     TMR_RX_DMACH
//----------------------------------------------
	 

//Communication speed setting for 1T
//----------------------------------------------
//#define  VSP_1TCLKRATE  48000000 //1T set to 48MHz
#define  VSP_1TCLKRATE  24000000 //1T clock rate set to 24MHz //tx by spi can work at this speed, but tx by timer can't work at this speed
//#define  VSP_1TCLKRATE  12000000 //1T clock rate set to 12MHz //STM32H7 tx by timer can work at this speed
//#define  VSP_1TCLKRATE  6000000 //1T clock rate set to 6MHz //STM32G0 tx by timer can work at this speed
//#define  VSP_1TCLKRATE  3000000 //1T clock rate set to 3MHz
//#define  VSP_1TCLKRATE  1500000 //1T clock rate set to 1.5MHz
//#define  VSP_1TCLKRATE  750000 //1T clock rate set to 750KHz
//----------------------------------------------


	 
//With support 256 bytes raw data transmission, buffer size need as below:
//----------------------------------------------
//with all 256 bytes data:128 (8 BMC bits: B10000000), total BMC pulse = 2564(BMCRXBUFF_SIZE), total BMC bytes = 610(BMCTXBUFF4SPI_SIZE)
//with all 256 bytes data:127 (7 BMC bits: B1111111), total BMC pulse = 3844(largest)(BMCRXBUFF_SIZE), total BMC bytes = 546(BMCTXBUFF4SPI_SIZE)
//with all 256 bytes data:0 (0 BMC bits), total BMC pulse = 260(least)(BMCRXBUFF_SIZE), total BMC bytes = 98(BMCTXBUFF4SPI_SIZE)

//with all 256 bytes data:128 (8 BMC bits: B10000000), total BMC pulse = 2564(BMCRXBUFF_SIZE), total BMC bytes = 4874(BMCTXBUFF4TMR_SIZE)
//with all 256 bytes data:127 (7 BMC bits: B1111111), total BMC pulse = 3844(largest)(BMCRXBUFF_SIZE), total BMC bytes = 4362(BMCTXBUFF4TMR_SIZE)
//with all 256 bytes data:0 (0 BMC bits), total BMC pulse(least) = 260(BMCRXBUFF_SIZE), total BMC bytes = 778(BMCTXBUFF4TMR_SIZE)

	 
//With support 128 bytes raw data transmission, buffer size need as below:
//----------------------------------------------
//with all 128 bytes data:128 (8 BMC bits: B10000000), total BMC pulse = 1284(BMCRXBUFF_SIZE), total BMC bytes = 306(BMCTXBUFF4SPI_SIZE)
//with all 128 bytes data:127 (7 BMC bits: B1111111), total BMC pulse = 1924(largest)(BMCRXBUFF_SIZE), total BMC bytes = 274(BMCTXBUFF4SPI_SIZE)
//with all 128 bytes data:0 (0 BMC bits), total BMC pulse = 132(least)(BMCRXBUFF_SIZE), total BMC bytes = 50(BMCTXBUFF4SPI_SIZE)

//with all 128 bytes data:128 (8 BMC bits: B10000000), total BMC pulse = 1284(BMCRXBUFF_SIZE), total BMC bytes = 2442(BMCTXBUFF4TMR_SIZE)
//with all 128 bytes data:127 (7 BMC bits: B1111111), total BMC pulse = 1924(largest)(BMCRXBUFF_SIZE), total BMC bytes = 2186(BMCTXBUFF4TMR_SIZE)
//with all 128 bytes data:0 (0 BMC bits), total BMC pulse(least) = 132(BMCRXBUFF_SIZE), total BMC bytes = 394(BMCTXBUFF4TMR_SIZE)
 
//#if 0
//With support 256 bytes raw data
//----------------------------------------------
 #define  TXRAWBUFF_SIZE  256
 #define  RXRAWBUFF_SIZE  256
	 
 #define  BMCTXBUFF4SPI_SIZE  612 //max need 610 when TXRAWBUFF_SIZE is 256
 #define  BMCTXBUFF4TMR_SIZE  4876 //max need 4874 when TXRAWBUFF_SIZE is 256
 #define  BMCRXBUFF_SIZE  3860 //max need 3844 when TXRAWBUFF_SIZE is 256
//#endif
	 
#if 0
//With support 128 bytes raw data
//----------------------------------------------
 #define  TXRAWBUFF_SIZE  128
 #define  RXRAWBUFF_SIZE  128

 #define  BMCTXBUFF4SPI_SIZE  308 //max need 306 when TXRAWBUFF_SIZE is 128
 #define  BMCTXBUFF4TMR_SIZE  2444 //max need 2442 when TXRAWBUFF_SIZE is 128
 #define  BMCRXBUFF_SIZE  1940 //max need 1924 when TXRAWBUFF_SIZE is 128
//----------------------------------------------
#endif
	 

	 
#if(VSP_TX_USED_SPI == 1)
 #define  BMCTXBUFF_SIZE     BMCTXBUFF4SPI_SIZE
#else //(VSP_TX_USED_TMR == 1)
 #define  BMCTXBUFF_SIZE     BMCTXBUFF4TMR_SIZE	
#endif //#if(VSP_TX_USED_SPI == 1)


#if(VSP_TX_ENABLE == 0)
 #define  BMCTXBUFF_SIZE  1 //for saving RAM usage
#endif //#if(VSP_TX_ENABLE == 1)

#if(VSP_RX_ENABLE == 0)
 #define  BMCRXBUFF_SIZE  1 //for saving RAM usage
#endif //#if(VSP_RX_ENABLE == 1)
	 


extern struct _DevVSPtx gDevVSPtx;
extern struct _DevVSPrx gDevVSPrx;




/************** EXPORTED FUNCTION *********************************************/

void VSP_LL_init(void);
void VSP_RX_LL_init(void);
u32 VSP_RX_LL_get_curr_buff_write_pt(struct _DevVSPrx *pDevVSPrx);
int VSP_TX_LL(struct _DevVSPtx *pDevVSPtx);
void VSP_TX_LL_cbFunc(enum _DevXferState devXferState, u32 nByteXfered);
void VSP_RX_LL_cbFunc(enum _DevXferState devXferState, u32 nByteXfered);
u16 VSP_LL_CRC16(const u8 *pData, u32 Len8);

/************** FOOTER ********************************************************/

#ifdef __cplusplus
}
#endif

#endif //__VSP_4_MCU_H

/*=========================== END OF FILE ====================================*/
