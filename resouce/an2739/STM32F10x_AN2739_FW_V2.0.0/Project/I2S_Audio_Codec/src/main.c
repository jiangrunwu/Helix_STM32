/**
  ******************************************************************************
  * @file I2S_Audio_Codec/src/main.c 
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     04/27/2009
  * @brief    This file provides I2S_Codec Application note firmware functions.
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

/** @addtogroup I2S_Audio_Codec
  * @{
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef USE_LCD
/* Information blocks */
uint8_t DemoTitle[20] = "STM32 I2S Codec Demo";

uint8_t CmdTitle0[20] = "  Control Buttons:  ";

uint8_t CmdTitle1Playing[20] = "KEY>Pause  UP  >Vol+";

uint8_t CmdTitle2Playing[20] = "SEL>Stop   DOWN>Vol-";

uint8_t CmdTitle1Paused[20] =  "KEY>Play   UP  >Spkr";

uint8_t CmdTitle2Paused[20] =  "SEL>Stop   DOWN>Head";

uint8_t CmdTitle1Stopped[20] = "    UP > Speaker    ";

uint8_t CmdTitle2Stopped[20] = "  DOWN > Headphone  ";

uint8_t StatusTitleStopped[20] = "      Stopped       ";

uint8_t StatusTitlePlaying[20] = "      Playing       ";

uint8_t StatusTitlePaused[20] = "       Paused       ";

/* Error messages */ 
uint8_t i2cerr[20] =  "ERROR:I2C com. ->RST";

uint8_t memerr[20] =  "ERROR: Memory  ->RST";

uint8_t fileerr[20] = "ERROR: File    ->RST";

/* Variable to store the previous progress status */
static uint8_t previoustmp = 50;
#endif /* USE_LCD */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void RCC_Config(void);
static void Joystick_Config(void);

#ifdef USE_LCD
static void I2S_CODEC_LCDConfig(void);
static void SysTick_Configuration(void);
#endif /* USE_LCD */

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{
  /* System Clocks Configuration */
  RCC_Config();

  /* Set the Interrupts priority group */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Choose number of repetitions: 0 => infinite repetitions */
  I2S_CODEC_ReplayConfig(0);

  /* Initialize the application environment */
  I2S_CODEC_Init(OutputDevice_SPEAKER, AUDIO_FILE_ADDRESS);

  /* Configure the Joystick related functionalities (EXTI, NVIC, GPIO) */
  Joystick_Config();

#ifdef USE_LCD
  /* Display the welcome screen and the commands */
  LCD_Update(ALL);

  /* Configure the SysTick for the Progress LCD information update */
  SysTick_Configuration();
#endif /* USE_LCD */

  /* Infinite loop */
  while (1)
  {}
}

/**
  * @brief  Configure the Joystick GPIOs and EXTI, NVIC channels.
  * @param  None
  * @retval : None
  */
void Joystick_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

  EXTI_DeInit();

  /* Configure PG.07, PG.08, PG.13, PG.14 and PG.15 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Configure PD.03 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource7);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource8);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource13);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource14);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource15);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource3);

  EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line7 | EXTI_Line8 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the EXTI3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI7 and  EXTI8 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the EXTI13, EXTI14 and EXTI15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef USE_LCD
/**
  * @brief  Initialize the LCD device and display the welcom screen.
  * @param  None
  * @retval : None
  */
void I2S_CODEC_LCDConfig(void)
{
  /* Initialize the LCD component */
  STM3210E_LCD_Init();

  Delay(0xFFFF);

  /* Set the text and the background color */
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(Black);
  LCD_Clear(White);

  Delay(0xFFFF);

  /* Display the Titles */
  LCD_DisplayStringLine(Line0, DemoTitle);
  LCD_DisplayStringLine(Line2, CmdTitle0);
}
#endif /* USE_LCD */

/**
  * @brief  Updates the display information.
  * @param atus: new status to be updated, could be: PLAY, PAUSE, STOP,
  *   PROGRESS, VOL or ALL.
  * @retval : None
  */
void LCD_Update(uint32_t Status)
{
#ifdef USE_LCD
  uint8_t tmp = 0;
  uint32_t counter = 0;

  /* Enable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  switch (Status)
  {
    case PROGRESS:
      tmp = (uint8_t) ((uint32_t)((GetVar_AudioDataIndex()) * 100) / GetVar_AudioDataLength());
      if (tmp == 0)
      {
        LCD_SetTextColor(Magenta);
        LCD_ClearLine(Line8);
        LCD_DrawRect(Line8, 310, 16, 300);
      }
      else
      {
        LCD_SetTextColor(Magenta);
        LCD_DrawLine(Line8, 310 - (tmp * 3), 16, Vertical);
      }
      break;
    case FRWRWD:
      tmp = (uint8_t) ((uint32_t)((GetVar_AudioDataIndex()) * 100) / GetVar_AudioDataLength());

      LCD_SetTextColor(Magenta);
      LCD_ClearLine(Line8);
      LCD_DrawRect(Line8, 310, 16, 300);
      LCD_SetTextColor(Magenta);

      for (counter = 0; counter <= tmp; counter++)
      {
        LCD_DrawLine(Line8, 310 - (counter * 3), 16, Vertical);
      }
      break;
    case STOP:
      /* Display the stopped status menu */
      LCD_SetTextColor(White);
      LCD_DisplayStringLine(Line3, CmdTitle1Stopped);
      LCD_DisplayStringLine(Line4, CmdTitle2Stopped);
      LCD_SetTextColor(Red);
      LCD_DisplayStringLine(Line6, StatusTitleStopped);
      LCD_ClearLine(Line9);
      LCD_SetTextColor(Black);
      LCD_DisplayChar(Line9, 250, 'v');
      LCD_DisplayChar(Line9, 235, 'o');
      LCD_DisplayChar(Line9, 220, 'l');
      LCD_DisplayChar(Line9, 200, '-');
      LCD_DisplayChar(Line9, 85, '+');
      LCD_DrawRect(Line9 + 8, 185, 10, 100);
      break;
    case PAUSE:
      /* Display the paused status menu */
      LCD_SetTextColor(White);
      LCD_DisplayStringLine(Line3, CmdTitle1Paused);
      LCD_DisplayStringLine(Line4, CmdTitle2Paused);
      LCD_SetTextColor(Red);
      LCD_DisplayStringLine(Line6, StatusTitlePaused);
      break;
    case PLAY:
      /* Display the Titles */
      LCD_SetTextColor(Black);
      LCD_DisplayStringLine(Line0, DemoTitle);
      LCD_DisplayStringLine(Line2, CmdTitle0);

      /* Display the Playing status menu */
      LCD_SetTextColor(White);
      LCD_DisplayStringLine(Line3, CmdTitle1Playing);
      LCD_DisplayStringLine(Line4, CmdTitle2Playing);
      LCD_SetTextColor(Red);
      LCD_DisplayStringLine(Line6, StatusTitlePlaying);
      LCD_ClearLine(Line9);
      LCD_SetTextColor(Black);
      LCD_DisplayChar(Line9, 250, 'v');
      LCD_DisplayChar(Line9, 235, 'o');
      LCD_DisplayChar(Line9, 220, 'l');
      LCD_DisplayChar(Line9, 200, '-');
      LCD_DisplayChar(Line9, 85, '+');
      LCD_DrawRect(Line9 + 8, 185, 10, 100);
      break;
    case ALL:
      I2S_CODEC_LCDConfig();
      /* Display the stopped status menu */
      LCD_SetTextColor(White);
      LCD_DisplayStringLine(Line3, CmdTitle1Stopped);
      LCD_DisplayStringLine(Line4, CmdTitle2Stopped);
      LCD_SetTextColor(Red);
      LCD_DisplayStringLine(Line6, StatusTitleStopped);
      LCD_ClearLine(Line9);
      LCD_SetTextColor(Black);
      LCD_DisplayChar(Line9, 250, 'v');
      LCD_DisplayChar(Line9, 235, 'o');
      LCD_DisplayChar(Line9, 220, 'l');
      LCD_DisplayChar(Line9, 200, '-');
      LCD_DisplayChar(Line9, 85, '+');
      LCD_DrawRect(Line9 + 8, 185, 10, 100);
      break;
  }
  /* Update the volume bar in all cases except when progress bar is to be apdated */
  if (Status != PROGRESS)
  {
    /* Compute the current volume percentage */
    tmp = (uint8_t) ((uint16_t)((0xFF - GetVar_CurrentVolume()) * 100) / 0xFF) ;

    /* Clear the previuos volume bar */
    LCD_SetTextColor(Blue);
    LCD_DrawLine(Line9 + 10, 185 - previoustmp , 8, Vertical);
    LCD_DrawLine(Line9 + 10, 185 - previoustmp + 1 , 8, Vertical);

    /* Draw the new volume bar */
    LCD_SetTextColor(Red);
    LCD_DrawLine(Line9 + 10, 185 - tmp, 8, Vertical);
    LCD_DrawLine(Line9 + 10, 185 - tmp + 1, 8, Vertical);

    /* save the current position */
    previoustmp = tmp;
  }
  /* Disable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
#endif /* USE_LCD */
}

/**
  * @brief  Configure the SysTick to generate an interrupt every ~1% of the
  *   audio file period.
  * @param  None
  * @retval : None
  */
void SysTick_Configuration(void)
{
  uint32_t reloadvalue = 0, err = 0;

  /* Initialize the audio file to determine the audio frequency
    and the Data Length in order to set SysTick reload value */
  err = AudioFile_Init();

  /* Check if the initialization passed */
  if ((err == 1) || (err == 2))
  {
    LCD_DisplayError(err);
  }
  /* Systick IT time = ~1% of the total audio stream duration:
     reloadvalue = ((((WaveFileLength/(2 * Fs)) / 100) * HCLK_Div8) / 2)
                 = (WaveFileLength / Fs) * 22500 */
  reloadvalue = (uint32_t)((GetVar_AudioDataLength() / (GetVar_i2saudiofreq())) * 20000);

  /* Check if the reload value overflows */
  if (reloadvalue >= 0xFFFFFF)
  {
    reloadvalue = 0xFFFFFF;
  }

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(reloadvalue))
  { 
    /* Capture error */ 
    while (1);
  }
  /* Configure and enable the SysTick peripheral */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Displays error message on the LCD screen and prompt user to
  *   reset the application.
  * @param r: Error code.
  * @retval : None
  */
void LCD_DisplayError(uint32_t err)
{
#ifdef USE_LCD
  I2S_CODEC_LCDConfig();

  LCD_SetTextColor(Red);
  LCD_ClearLine(Line7);

  /* The memory initialazation failed */
  if (err == 1)
  {
    LCD_DisplayStringLine(Line7, memerr);
  }

  /* The audio file initialization failed (wrong audio format or wrong file) */
  if (err == 2)
  {
    LCD_DisplayStringLine(Line7, fileerr);
  }

  /* I2C communication failure occured */
  if (err == 3)
  {
    LCD_DisplayStringLine(Line7, i2cerr);
  }
#endif /* USE_LCD */

  /* Enter infinite loop waiting user to reset system */
  while (1)
  {}
}

/**
  * @brief  Inserts a delay time.
  * @param ount: Number of cycles to be counted to make a delay.
  * @retval : None
  */
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval : None
  */
void RCC_Config(void)
{
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Executes an infinite loop when wrong parameter is entered to
  *   a lib function.
  * @param le: pointer to the source file name
  *   line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
