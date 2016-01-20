#include "stm32f10x.h"
#include "df_mp3play.h"
#include "audioplay.h"
#include "stdio.h"
#include "common.h"
#include "stm32f10x_spi.h"
#include "wm8978.h"
#include "df_i2s.h"

__IO uint32_t TIM2_TimingMillis;
__audiodev audiodev;



//I2S Txbuffer
 
uint16_t I2STxBuf[2048] = {

   0x18FF,0x18FF,0x18FF,0x18FF,0x0,0x0,0x0,0x0
};



void  I2S_send_data(uint16_t *txbuf, int size){
    
   int RxIdx = 0;
   int TxIdx = 0;
     
     /* Begin the communication in I2S mode */
    while (TxIdx < size)
    {
        /* Wait the Tx buffer to be empty */
        while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
        {}
        /* Send a data from I2S3 */
        SPI_I2S_SendData(SPI2, txbuf[TxIdx++]);
      
#if 0
      /* Wait the Rx buffer to be full */
      while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
      {}
      /* Store the I2S2 received data in the relative data table */
      I2S2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
#endif
    }

}



int main(void)
{

    FRESULT res;
    FIL fs;
    char buf2[100];

    uint32_t tmpreg;

    bsp_init();

/*
    res = f_open(&fs, "1:alarm4", FA_CREATE_NEW | FA_WRITE | FA_READ);

    f_close(&fs);

    
*/
    // res = f_open(&fs, "1:alarm4",FA_READ);
    // res = f_lseek(&fs, 0x2fda0);
     //res = f_read(&fs, I2STxBuf, 2048, &tmpreg);
         
    //mp3_play_song("1:wait.mp3");
    WM8978_I2S_Cfg(2,0);	//飞利浦标准,16位数据长度
    I2S2_Init(I2S_Standard_Phillips, I2S_MCLKOutput_Enable, mp3ctrl->samplerate);
  
    while (1)
     I2S_send_data(I2STxBuf, 8);
    
        ;

    return 0;


}














