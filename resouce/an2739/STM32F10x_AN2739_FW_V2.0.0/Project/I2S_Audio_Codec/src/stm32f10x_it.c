/**
  ******************************************************************************
  * @file I2S_Audio_Codec/src/stm32f10x_it.c 
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     04/27/2009
  * @brief    Main Interrupt Service Routines.
  *           This file provides template for all exceptions handler and 
  *           peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "i2s_codec.h"
#include "main.h"

/** @addtogroup I2S_Audio_Codec
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
  /* Update the Progress bar display */
  LCD_Update(PROGRESS);
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles External interrupt Line 3 request.
  * @param  None
  * @retval : None
  */
void EXTI3_IRQHandler(void)
{
  uint32_t err = 0;

  /* DOWN button pushed => Decrease Volume/Headphone Enable -------------*/
  if (EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line3);

    /* If the Codec is PLAYING => Decrease Volume*/
    if (GetVar_AudioPlayStatus() == AudioPlayStatus_PLAYING)
    {
      /* Increase the audio codec digital volume */
      I2S_CODEC_ControlVolume(VolumeDirection_LOW, VOLStep);

      /* Update the LCD display */
      LCD_Update(VOL);
    }

    else /* If the Codec is PAUSED => Headphone Enable */
    {
      /* Update the LCD display */
      LCD_Update(PLAY);

      /* Enable the Headphone output and reinitialize all devices */
      err = I2S_CODEC_SpeakerHeadphoneSwap(OutputDevice_HEADPHONE, AUDIO_FILE_ADDRESS);

      /* Error message display if failure */
      if (err != 0)
      {
        LCD_DisplayError(err);
      }
    }
  }
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval : None
  */
void EXTI9_5_IRQHandler(void)
{
  uint32_t err = 0;

  /* User Buttomn pushed => PLAY/PAUSE ---------------------------------------*/
  if (EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line8);

    /* If the Codec is Playing => PAUSE */
    if (GetVar_AudioPlayStatus() == AudioPlayStatus_PLAYING)
    {
      /* Update the display information */
      LCD_Update(PAUSE);

      /* Command the Pause of the current stream */
      SetVar_AudioPlayStatus(AudioPlayStatus_PAUSED);
    }

    /* If the Codec is PAUSED => Resume PLAYING */
    else if (GetVar_AudioPlayStatus() == AudioPlayStatus_PAUSED)
    {
      /* Update the LCD display */
      LCD_Update(PLAY);

      /* Start playing from the last saved position */
      I2S_CODEC_Play(GetVar_AudioDataIndex());
    }

    /* If the Codec is STOPPED => PLAY from the file start address */
    else if (GetVar_AudioPlayStatus() == AudioPlayStatus_STOPPED)
    {
      /* Update the display information */
      LCD_Update(PLAY);

      /* Initialize all devices w/choosen parameters */
      err = I2S_CODEC_Init(GetVar_CurrentOutputDevice(), AUDIO_FILE_ADDRESS);

      /* Error message display if failure */
      if (err != 0)
      {
        LCD_DisplayError(err);
      }

      /* Enable Playing the audio file */
      I2S_CODEC_Play(GetVar_DataStartAddr());
    }
  }

  /* SEL button pushed => STOP ----------------------------------------------*/
  if (EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line7);

    /* Update the display information */
    LCD_Update(STOP);

    /* Command the Stop of the current audio stream */
    SetVar_AudioPlayStatus(AudioPlayStatus_STOPPED);
  }
}

/**
  * @brief  This function handles SPI2 global interrupt request.
  * @param  None
  * @retval : None
  */
void SPI2_IRQHandler(void)
{
  if ((SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET))
  {
    /* Send data on the SPI2 and Check the current commands */
    I2S_CODEC_DataTransfer();
  }
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval : None
  */
void EXTI15_10_IRQHandler(void)
{
  uint32_t err = 0;

  /* UP button pushed => Increase Volume level ------------------------------*/
  if (EXTI_GetITStatus(EXTI_Line15) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line15);

    /* Check if the Codec is PLAYING audio file */
    if (GetVar_AudioPlayStatus() == AudioPlayStatus_PLAYING)
    {
      /* Increase the audio codec digital volume */
      I2S_CODEC_ControlVolume(VolumeDirection_HIGH, VOLStep);

      /* Update the display information */
      LCD_Update(VOL);
    }
    /* UP button pushed in PAUSE mode => Enable the Speaker device output ---*/
    else
    {
      /* Update the display information */
      LCD_Update(PLAY);

      /* Configure the Speaker as output and reinitialize all devices */
      err = I2S_CODEC_SpeakerHeadphoneSwap(OutputDevice_SPEAKER, AUDIO_FILE_ADDRESS);

      /* Error message display if failure */
      if (err != 0)
      {
        LCD_DisplayError(err);
      }
    }
  }

  /* Right button pushed => Forward and play ----------------------------------*/
  if (EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line13);

    /* Check if the Codec is PLAYING audio file */
    if (GetVar_AudioPlayStatus() == AudioPlayStatus_PLAYING)
    {
      I2S_CODEC_ForwardPlay(STEP_FORWARD);

      /* Update the display information */
      LCD_Update(FRWRWD);
    }
  }

  /* Left button pushed => Rewind and play ----------------------------------*/
  if (EXTI_GetITStatus(EXTI_Line14) != RESET)
  {
    /* Clear the interrupt source pending */
    EXTI_ClearITPendingBit(EXTI_Line14);

    /* Check if the Codec is PLAYING audio file */
    if (GetVar_AudioPlayStatus() == AudioPlayStatus_PLAYING)
    {
      I2S_CODEC_RewindPlay(STEP_BACK);

      /* Update the display information */
      LCD_Update(FRWRWD);
    }
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
