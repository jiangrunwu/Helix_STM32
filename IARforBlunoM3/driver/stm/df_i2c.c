#include "df_i2c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stdbool.h"

//初始化IIC
//SCL: PB11
//SDA: PB10



#define I2C_READ               1
#define I2C_WRITE             0
#define EOTHER                4

void DF_IIC_Init(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	SDA_HIGH();
	SCL_HIGH();
    
}
//产生IIC起始信号
void DF_IIC_Start(void)
{
	SDA_HIGH();
	SCL_HIGH();
	I2C_DELAY();
 	SDA_LOW();//START:when CLK is high,DATA change form high to low 
	I2C_DELAY();
	SCL_LOW();//钳住I2C总线，准备发送或接收数据 
    
}	  
//产生IIC停止信号
void DF_IIC_Stop(void)
{
    SDA_LOW();//STOP:when CLK is high DATA change form low to high
 	SCL_LOW();
    I2C_DELAY();
    SCL_HIGH();//发送I2C总线结束信号
	I2C_DELAY();
    SDA_HIGH();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
bool DF_IIC_Wait_Ack(void)
{
	uint32_t ucErrTime=0;
    
    I2C_DELAY();
	SDA_HIGH();
    I2C_DELAY();
	SCL_HIGH();
    I2C_DELAY(); 
    
	while(SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			DF_IIC_Stop();
			return false;
		}
	}
	SCL_LOW();//时钟输出0 	   
	return true;  
} 
//产生ACK应答
void DF_IIC_Ack(void)
{
    I2C_DELAY();
	SDA_LOW();
    I2C_DELAY();
	SCL_HIGH();
    I2C_DELAY();
	SCL_LOW();
}
//不产生ACK应答		    
void DF_IIC_NAck(void)
{
	I2C_DELAY();
	SDA_HIGH();
    I2C_DELAY();
    SCL_HIGH();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void DF_IIC_Send_Byte(u8 txd)
{   int i;
	//Serial.begin(9600);

	//digitalWrite(port.scl,LOW);
    for (i=0;i<8;i++) {
        
        if ((txd & (1 << (7 - i))))
           
            GPIO_SetBits(GPIOB, GPIO_Pin_10);
        else
           GPIO_ResetBits(GPIOB, GPIO_Pin_10);
        
        I2C_DELAY();
        SCL_HIGH();
        I2C_DELAY();
        SCL_LOW();
		
    } 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t DF_IIC_Read_Byte()
{
	 uint8_t data = 0;
     int i;
     
     for (i=0;i<8;i++) {
        I2C_DELAY();
        SCL_HIGH();
        I2C_DELAY();
        data += SDA_READ() << (7-i);
        I2C_DELAY();
        SCL_LOW();
    }

    return data;
}


char i2c_shift_outonebyte(uint8_t Saddr, uint16_t Iaddr, uint8_t data)
{
   char temp = 0xFF;
   
  
    DF_IIC_Start();
    I2C_DELAY();
    DF_IIC_Send_Byte(Saddr << 1 | I2C_WRITE);
    I2C_DELAY();
    temp = DF_IIC_Wait_Ack();
    DF_IIC_Send_Byte((uint8_t)((Iaddr >> 8) & 0x7F));
    I2C_DELAY();
    if (!(temp = DF_IIC_Wait_Ack()))
    {
        return EOTHER;
    
    }

    DF_IIC_Send_Byte((uint8_t)(Iaddr & 0xFF));
    I2C_DELAY();
     if (!(temp = DF_IIC_Wait_Ack()))
    {
        return EOTHER;
    
    }
    
    DF_IIC_Send_Byte(data);
    if (!(temp = DF_IIC_Wait_Ack()))
    {
        return EOTHER;
    
    }

    DF_IIC_Stop();
    I2C_DELAY();
	
    return SUCCESS;

}

char i2c_shift_inonebyte(uint8_t Saddr, uint16_t Iaddr)
{
    char temp = 0xFF;
    
    DF_IIC_Start();
    DF_IIC_Send_Byte(Saddr << 1 | I2C_WRITE);
    temp = DF_IIC_Wait_Ack();


    DF_IIC_Send_Byte((uint8_t)((Iaddr >> 8) & 0x7F));
    temp = DF_IIC_Wait_Ack();

    DF_IIC_Send_Byte((uint8_t)(Iaddr & 0xFF));
    temp =  DF_IIC_Wait_Ack();

    DF_IIC_Start();
    DF_IIC_Send_Byte(Saddr << 1 | I2C_READ);
    temp = DF_IIC_Wait_Ack();

    temp = DF_IIC_Read_Byte();
    DF_IIC_NAck();

    DF_IIC_Stop();
    
    return temp;
}























void test_eeprom(void)
{
  char data[10] = {11, 2, 3, 4,5,6,7, 10, 20, 32}, dataread[10];
   int ret;
   
   I2c_Init();
   I2C_DELAY();
   while (1)
   {
     for (int i = 0; i < 10; i++)
        ret = (int)i2c_shift_outonebyte(0x50, 0x0 + i, data[i]);


     for (int i = 0; i < 10; i++)
        dataread[i] = i2c_shift_inonebyte(0x50, 0x0+i);
     if (memcmp(data, dataread, 10))
        I2C_DELAY();
     
     
     I2C_DELAY();
   }



}




















