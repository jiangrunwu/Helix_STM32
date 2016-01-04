#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"


 
 				  	    													  
void SPIx_Init(void);			 //初始化SPI口
void SPIx_SetSpeed(uint8_t SpeedSet); //设置SPI速度   
u8 SPIx_ReadWriteByte(uint8_t TxData);//SPI总线读写一个字节
		 
#endif

