/**
  @page I2S_Audio_Codec AN2739 I2S_Audio_Codec Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2009 STMicroelectronics *******************
  * @file     I2S_Audio_Codec/readme.txt 
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     04/27/2009
  * @brief    The AN2739 application note describes how to use the high-density 
  *           devices I2S feature to play audio files using an external codec.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description
 
This application note describes how to use the high-density devices I2S feature 
to play audio files using an external codec.

@par Directory contents 

+ inc 
    - I2S_Audio_Codec/inc/fonts.h           LCD fonts size definition
    - I2S_Audio_Codec/inc/fsmc_nor.h        FSMC NOR driver Header file
    - I2S_Audio_Codec/inc/i2s_codec.h       I2S codec driver Header file 
    - I2S_Audio_Codec/inc/stm3210e_lcd.h    LCD Header file
    - I2S_Audio_Codec/inc/main.h            Main Header file
    - I2S_Audio_Codec/inc/spi_flash.h       SPI Flash driver Header file
    - I2S_Audio_Codec/inc/stm32f10x_conf.h  Library Configuration file   
    - I2S_Audio_Codec/inc/stm32f10x_it.h    Interrupt handlers header file      

+ src 
    - I2S_Audio_Codec/src/fsmc_nor.c        FSMC NOR driver firmware functions
    - I2S_Audio_Codec/src/i2s_codec.c       I2S codec driver firmware functions
    - I2S_Audio_Codec/src/main.c            Main program
    - I2S_Audio_Codec/src/stm3210e_lcd.c    LCD driver firmware functions
    - I2S_Audio_Codec/src/spi_flash.c       SPI Flash drvier firmware functions
    - I2S_Audio_Codec/src/stm32f10x_it.c    Interrupt handlers  


@par Hardware and Software environment 

  - This example runs on STM32F10x High-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (STM32F10x 
    High-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.

  - STM3210E-EVAL Set-up 
    - Since the MCLK feature is enabled, configure MCKI Jumper (JP18) in the 2<>3
      position to connect the I2S2 MCLK signal to the MCLKI codec pin.
      
@note
  If the audio file to be played has a big size, it should be previously loaded 
  into the memory source (NOR Flash memory or SPI Flash) using an independent 
  application (IAP, DFU, etc.). It may also be included as a table file.

@par How to use it ?  

 - RVMDK 
    - Open the I2S_Audio_Codec.Uv2 project
    - In the build toolbar select the project config:
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 - EWARM5 
    - Open the I2S_Audio_Codec.eww workspace.
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - RIDE 
    - Open the I2S_Audio_Codec.rprj project.
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)  

@note
- High-density devices are STM32F101xx and STM32F103xx microcontrollers 
  where the Flash memory density ranges between 256 and 512 Kbytes.

 * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
 */
