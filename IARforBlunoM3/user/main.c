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

    res = f_open(&fs, "1:alarm123", FA_CREATE_NEW | FA_WRITE | FA_READ);

    f_close(&fs);

    mp3_play_song("1:wang.mp3");

    while (1)

        ;

    return 0;


}














