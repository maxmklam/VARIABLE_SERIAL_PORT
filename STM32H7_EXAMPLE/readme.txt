/*==============================================================================
 @FILE: readme.txt
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
 
 This file describes the use of projects STM32H750VB_VSP_MASTER_EXAMPLE and STM32H750VB_VSP_CLIENT_EXAMPLE for VSP transmission and reception example.
 The project can only be oponed with IAR 9.xx
 
 Structure of folder/file:
 myLIB\ -- Contains all platform independent files which implement a blocking time delay function (in us resolution), and a non-blocking time elapse measurement function (in us resolution).
 VSP\ -- Contains all platform independent files which implement all the VSP transmission and reception functions.
 Core\ -- Contains platform dependent files generate by STM32CUBEMX, most are peripheral control related.
 Drivers\ -- Driver files generate by STM32CUBEMX.
 myCode_MCU\ -- Additional platform dependent files for that MCU of that project.
 myCode_MCU\VSP_4_MCU\ -- Contains platform dependent files for the VSP\ folder.
 myCode_STM32H7\ -- Contains all the files commonly accesible for all STM32H7 series MCU for all projects.
 myCode_STM32H7\myLIB_4_STM32H7\ -- Contains platform dependent files for the myLIB\ folder
==============================================================================*/

Approach 1:
On master side, transmission using SPI + DMA
On client side, reception using TIMER input capture + DMA

How to enable:
in STM32H750VB_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h, 
#define  VSP_TX_USED_SPI  1 //0: VSP_TX_USED_TMR

   __________________________                       __________________________
  |         |    VSP tx      |                      |    VSP rx     |         |
  |         |                |                      |               |         |
  |         | (PB.8)SPI2 MOSI|--------------------->|TIM1 CC1(PE.9) |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |________________|                      |_______________|         |
  |                          |                      |                         |
  |                          |                      |                         |
  |    STM32H750VB master    |                      |    STM32H750VB client   |
  |                       GND|______________________|GND                      |
  |                          |                      |                         |
  |__________________________|                      |_________________________|



How the VSP transmission using SPI + DMA works:
Raw data (max 128 bytes) is passed into VSP_TX_data2bmc_encode_spi(), encoded in BMC format bit by bit, then packed byte by byte and saved in bmcTxBuff[] and sent by SPI MOSI pin through DMA




Approach 2:
On master size, transmission using TIMER output compare + DMA
On client size, reception using TIMER input capture + DMA

How to enable:
in STM32H750VB_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h, 
#define  VSP_TX_USED_SPI  0 //0: VSP_TX_USED_TMR

   __________________________                       __________________________
  |         |    VSP tx      |                      |    VSP rx     |         |
  |         |                |                      |               |         |
  |         | (PA.6)TIM16 CC1|--------------------->|TIM1 CC1(PE.9) |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |                |                      |               |         |
  |         |________________|                      |_______________|         |
  |                          |                      |                         |
  |                          |                      |                         |
  |    STM32H750VB master    |                      |    STM32H750VB client   |
  |                       GND|______________________|GND                      |
  |                          |                      |                         |
  |__________________________|                      |_________________________|



How the VSP transmission using TIMER output compare + DMA works:
Raw data (max 128 bytes) is passed into VSP_TX_data2bmc_encode_tmr(), encoded in BMC format, one BMC bit requires one byte to represent, then saved in bmcTxBuff[] and sent by TIMER output compare pin by force active mode through DMA.
In this approach, because one BMC bit needed one byte of data, that's why the bmcTxBuff[] buffer size is much larger than using SPI approach.
Data contained in bmcTxBuff[] are look likes: 0x48, 0x58 which is written to TIMER register: CCMR1 to make the TIMER output compare pin become "low" or "high" respectively. Using this approach to continueously output BMC waveform on the TIMER output compare pin.


Since STM32H750VB has pretty enough RAM resource, so both STM32H750VB_VSP_MASTER_EXAMPLE and STM32H750VB_VSP_CLIENT_EXAMPLE support VSP tx/rx at the same time.
Done by enabling VSP_TX_ENABLE and VSP_RX_ENABLE at the same time in STM32H750VB_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h and, 
STM32H750VB_VSP_CLIENT_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h 

#define  VSP_TX_ENABLE  1
#define  VSP_RX_ENABLE  0
