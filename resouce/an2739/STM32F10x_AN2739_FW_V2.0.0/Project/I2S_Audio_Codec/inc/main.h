/**
  ******************************************************************************
  * @file I2S_Audio_Codec/inc/main.h 
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     04/27/2009
  * @brief    Header file for main.c.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "spi_flash.h"
#include "i2s_codec.h"
#include "stm3210e_lcd.h"
#include "fsmc_nor.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment this line to use the LCD for the information display */
#define USE_LCD

/* Audio file location */
#define AUDIO_FILE_ADDRESS  0x64060000

/* LCD display update parameters */
#define STOP        0x10000000
#define PLAY        0x01000000
#define PAUSE       0x00100000
#define VOL         0x00010000
#define PROGRESS    0x00001000
#define FRWRWD      0x00000100
#define ALL         0x11110100

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* General Purpose Configuration Functions */
void LCD_Update(uint32_t Status);
void LCD_DisplayError(uint32_t err);
void Delay(__IO uint32_t nCount);

#endif /* __MAIN_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
