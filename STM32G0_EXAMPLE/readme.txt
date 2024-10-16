/*==============================================================================
 @FILE: readme.txt
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
 
 This file describes the use of projects STM32G030F6_VSP_MASTER_EXAMPLE and STM32G030F6_VSP_CLIENT_EXAMPLE for VSP transmission and reception example.
==============================================================================*/

Approach 1:
On master size, transmission using SPI + DMA
On client size, reception using TIMER input capture + DMA

How to enable:
in STM32G030F6_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h, 
#define  VSP_TX_USED_SPI  1 //0: VSP_TX_USED_TMR

   __________________________                       __________________________
  |         |    VSP tx      |                      |    VSP rx     |         |
  |         |                |                      |               |         |
  |         | (PA.4)SPI2 MOSI|--------------------->|TIM1 CC1(PA.8) |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |________________|                      |_______________|         |
  |                          |                      |                         |
  |                          |                      |                         |
  |    STM32G030F6 master    |                      |    STM32G030F6 client   |
  |                       GND|______________________|GND                      |
  |                          |                      |                         |
  |__________________________|                      |_________________________|






Approach 2:
On master size, transmission using TIMER output compare + DMA
On client size, reception using TIMER input capture + DMA

How to enable:
in STM32G030F6_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h, 
#define  VSP_TX_USED_SPI  0 //0: VSP_TX_USED_TMR

   __________________________                       __________________________
  |         |    VSP tx      |                      |    VSP rx     |         |
  |         |                |                      |               |         |
  |         | (PA.6)TIM16 CC1|--------------------->|TIM1 CC1(PA.8) |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |________________|                      |_______________|         |
  |                          |                      |                         |
  |                          |                      |                         |
  |    STM32G030F6 master    |                      |    STM32G030F6 client   |
  |                       GND|______________________|GND                      |
  |                          |                      |                         |
  |__________________________|                      |_________________________|





Basically both STM32G030F6_VSP_MASTER_EXAMPLE and STM32G030F6_VSP_CLIENT_EXAMPLE support VSP tx/rx, however, due to limits of RAM size, VSP tx/rx can't be enabled in one MCU at the same time.
So you can only just select tx or rx each time, in STM32G030F6_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h, and
STM32G030F6_VSP_CLIENT_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h

#define  VSP_TX_ENABLE  1
#define  VSP_RX_ENABLE  0


If you really want to do VSP tx/rx in the same MCU, you can reduce the TXRAWBUFF_SIZE and RXRAWBUFF_SIZE. Currently maximum buffer size for transmitting/receiving raw data is set to 128 byte.
 
//With support 128 bytes raw data
//----------------------------------------------
 #define  TXRAWBUFF_SIZE  128
 #define  RXRAWBUFF_SIZE  128

 #define  BMCTXBUFF4SPI_SIZE  308 //max need 306 when TXRAWBUFF_SIZE is 128
 #define  BMCTXBUFF4TMR_SIZE  2444 //max need 2442 when TXRAWBUFF_SIZE is 128
 #define  BMCRXBUFF_SIZE  1940 //max need 1924 when TXRAWBUFF_SIZE is 128
//----------------------------------------------



 
