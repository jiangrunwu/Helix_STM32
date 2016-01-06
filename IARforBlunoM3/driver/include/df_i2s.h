#ifndef __DF_I2S_H
#define __DF_I2S_H


#include "stm32f10x.h"




extern void (*i2s_tx_callback)(void);		//IIS TX回调函数指针 
extern void (*i2s_rx_callback)(void);		//IIS RX回调函数指针 


void I2S2_Init(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq); 
void I2S2ext_Init(uint8_t std,uint8_t mode,uint8_t cpol,uint8_t datalen);
uint8_t I2S2_SampleRate_Set(uint32_t samplerate);

void I2S2_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num);



void I2S2ext_RX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num);
void I2S_Play_Start(void);
void I2S_Rec_Start(void);
void I2S_Play_Stop(void);
void I2S_Rec_Stop(void);
 

#endif
