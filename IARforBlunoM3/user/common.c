#include "ffconf.h"
#include "ff.h"
#include "df_i2c.h"
#include "df_i2s.h"
#include "wm8978.h"






extern int DF_Usart1Init(void);



FATFS fs;

int  bsp_init(void){

    FRESULT tmp;
    int res = 0;
   
    RCC_ClocksTypeDef RCC_Clocks;

    //get the clocksfreq.
    RCC_GetClocksFreq(&RCC_Clocks); 
    
   
    WM8978_Init();
   

	WM8978_ADDA_Cfg(1,0);
	WM8978_Input_Cfg(0,0,0);
	WM8978_Output_Cfg(1,0);
    
    
 
    
    tmp = f_mount(&fs, "1:", 1); //第三个参数在制作文件系统时传‘0’，表示随后在mount， 当flash有文件系统时则传'1'.表示马上mount。
    if (tmp != FR_OK){
        
        tmp = -1;
    
    }
   
    //initailize the usart1
    DF_Usart1Init();    
    
   
    return res;

}
