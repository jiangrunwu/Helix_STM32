#include "stm32f10x.h"
#include "df_mp3play.h"
#include "audioplay.h"
#include "stdio.h"
#include "common.h"



 __IO uint32_t TIM2_TimingMillis;
__audiodev audiodev;

int main(void)
{
  
    FRESULT res;
    FIL fs;
    char buf2[100];
    uint32_t tmpreg;

    bsp_init();
    DF_SendChar('n');
    DF_SendChar('i');
    DF_SendChar('h');
    DF_SendChar('a');
    DF_SendChar('o');
    printf("hello world\n");
    mp3_play_song("1:liuxijun.mp3");

    while (1)
      
      ;
  






  return 0;


}














