#include "df_i2s.h"  
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"






static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* SPI2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                          RCC_APB2Periph_AFIO, ENABLE);

  /* I2S2 SD, CK and WS pins configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* I2S2 MCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}



/**
  * @brief  Configure the I2S Peripheral.
  * @param  Standard: I2S_Standard_Phillips, I2S_Standard_MSB or I2S_Standard_LSB
  * @param MCLKOutput: I2S_MCLKOutput_Enable or I2S_MCLKOutput_Disable
  * @param AudioFreq: I2S_AudioFreq_8K, I2S_AudioFreq_16K, I2S_AudioFreq_22K,
  *   I2S_AudioFreq_44K or I2S_AudioFreq_48K
  * @retval : None
  */
void I2S2_Init(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq)
{

  I2S_InitTypeDef I2S_InitStructure;


   /* Configure I2S interrupt Channel */
 // NVIC_Config();
  
  //config the GPIO pin for I2S
  GPIO_Config();
  
  /* Enable I2S2 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* Deinitialize SPI2 peripheral */
  SPI_I2S_DeInit(SPI2);

  /* I2S2 peripheral configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = Standard;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = MCLKOutput;
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &I2S_InitStructure);

  /* Disable the I2S2 TXE Interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);

  /* Enable the SPI2/I2S2 peripheral */
  I2S_Cmd(SPI2, ENABLE);


} 





//I2S2ext配置,用于全双工模式配置,需要时,在I2S2_Init之后调用
//std:I2S标准,00,飞利浦标准;01,MSB对齐标准(右对齐);10,LSB对齐标准(左对齐);11,PCM标准
//mode:I2S工作模式,00,从机发送;01,从机接收;I2S2ext仅用于从机模式
//cpol:0,时钟低电平有效;1,时钟高电平有效
//datalen:数据长度,0,16位标准;1,16位扩展(frame=32bit);2,24位;3,32位.
void I2S2ext_Init(uint8_t std,uint8_t mode,uint8_t cpol,uint8_t datalen)
{  
	
} 

//采样率计算公式:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
//I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
//一般HSE=8Mhz 
//pllm:在Sys_Clock_Set设置的时候确定，一般是8
//PLLI2SN:一般是192~432 
//PLLI2SR:2~7
//I2SDIV:2~255
//ODD:0/1
//I2S分频系数表@pllm=8,HSE=8Mhz,即vco输入频率为1Mhz
//表格式:采样率/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
const uint16_t I2S_PSC_TBL[][5]=
{
	{800 ,256,5,12,1},		//8Khz采样率
	{1102,429,4,19,0},		//11.025Khz采样率 
	{1600,213,2,13,0},		//16Khz采样率
	{2205,429,4, 9,1},		//22.05Khz采样率
	{3200,213,2, 6,1},		//32Khz采样率
	{4410,271,2, 6,0},		//44.1Khz采样率
	{4800,258,3, 3,1},		//48Khz采样率
	{8820,316,2, 3,1},		//88.2Khz采样率
	{9600,344,2, 3,1},  	//96Khz采样率
	{17640,361,2,2,0},  	//176.4Khz采样率 
	{19200,393,2,2,0},  	//192Khz采样率
};  
//设置IIS的采样率(@MCKEN)
//samplerate:采样率,单位:Hz
//返回值:0,设置成功;1,无法设置.
uint8_t I2S2_SampleRate_Set(uint32_t samplerate)
{ 
	return 0;
}  
//I2S2 TX DMA配置
//设置为双缓冲模式,并开启DMA传输完成中断
//buf0:M0AR地址.
//buf1:M1AR地址.
//num:每次传输数据量
void I2S2_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num)
{  
	
} 
//I2S2ext RX DMA配置
//设置为双缓冲模式,并开启DMA传输完成中断
//buf0:M0AR地址.
//buf1:M1AR地址.
//num:每次传输数据量
void I2S2ext_RX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num)
{  
	
} 

//I2S DMA回调函数指针
void (*i2s_tx_callback)(void);	//TX回调函数
void (*i2s_rx_callback)(void);	//RX回调函数

//DMA1_Stream4中断服务函数
void DMA1_Stream4_IRQHandler(void)
{      
								 
} 
//DMA1_Stream3中断服务函数
void DMA1_Stream3_IRQHandler(void)
{      
											 
} 
//I2S开始播放
void I2S_Play_Start(void)
{   	  
	//DMA1_Stream4->CR|=1<<0;		//开启DMA TX传输  		
}
//关闭I2S播放
void I2S_Play_Stop(void)
{   	 
	//DMA1_Stream4->CR&=~(1<<0);	//结束播放	 	 
}
//I2S开始录音
void I2S_Rec_Start(void)
{   	    
	//DMA1_Stream3->CR|=1<<0;		//开启DMA RX传输 		
}
//关闭I2S录音
void I2S_Rec_Stop(void)
{   	  
	//DMA1_Stream3->CR&=~(1<<0);	//结束录音		 
}








