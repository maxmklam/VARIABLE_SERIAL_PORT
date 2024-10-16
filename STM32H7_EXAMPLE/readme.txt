/*==============================================================================
 @FILE: readme.txt
 @AUTHOR: maxmklam
 @OWNER: maxmklam
 @DESCRIBE: 
 Copyright (c) 2024 maxmklam 
 
 This file describes the use of projects STM32H750VB_VSP_MASTER_EXAMPLE and STM32H750VB_VSP_CLIENT_EXAMPLE for VSP transmission and reception example.
==============================================================================*/

Approach 1:
On master size, transmission using SPI + DMA
On client size, reception using TIMER input capture + DMA

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





Since STM32H750VB has pretty enough RAM resource, so both STM32H750VB_VSP_MASTER_EXAMPLE and STM32H750VB_VSP_CLIENT_EXAMPLE support VSP tx/rx at the same time.
Done by enabling VSP_TX_ENABLE and VSP_RX_ENABLE at the same time in STM32H750VB_VSP_MASTER_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h and, 
STM32H750VB_VSP_CLIENT_EXAMPLE\myCode_MCU\VSP_4_MCU\VSP_4_MCU.h 

#define  VSP_TX_ENABLE  1
#define  VSP_RX_ENABLE  0
