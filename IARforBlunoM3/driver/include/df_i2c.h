#ifndef __DF_I2C_H
#define __DF_I2C_H

#include "stm32f10x_gpio.h"
#include "stdbool.h"

#define SDA_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define SDA_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define SDA_READ()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)

#define SCL_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define SCL_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define I2C_DELAY() do{for(int i=0;i<100 ;i++) {asm volatile("nop");}}while(0)

   	   	
//IIC所有操作函数

void DF_IIC_Init(void);                //初始化IIC的IO口				 
void DF_IIC_Start(void);				//发送IIC开始信号
void DF_IIC_Stop(void);	  			//发送IIC停止信号
void DF_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t DF_IIC_Read_Byte();//IIC读取一个字节
bool DF_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void DF_IIC_Ack(void);					//IIC发送ACK信号
void DF_IIC_NAck(void);				//IIC不发送ACK信号 
#endif


