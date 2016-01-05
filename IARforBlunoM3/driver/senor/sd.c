#include "SPI.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"



#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   

#define CMD0    0      
#define CMD1    1
#define CMD8    8     
#define CMD9    9       
#define CMD10   10      
#define CMD12   12     
#define CMD16   16      
#define CMD17   17    
#define CMD18   18    
#define CMD23   23   
#define CMD24   24     
#define CMD25   25   
#define CMD41   41     
#define CMD55   55    
#define CMD58   58    
#define CMD59   59      
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD?¡§??¨®|¡À¨º??¡Á?
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

#define HIGH            1
#define LOW            0


void digitalWrite( uint32_t ulPin, uint32_t ulVal ){


  if (ulVal == HIGH)
      GPIO_SetBits(GPIOB, GPIO_Pin_5);
  else
      GPIO_ResetBits(GPIOB, GPIO_Pin_5);

}

const int slaveSelectPin = 4;
uint8_t  SD_Type=0;

uint8_t SD_WaitReady(void)
{
  u32 t=0;
  do
  {
    if(SPIx_ReadWriteByte(0xff)==0xff)
    {
      return 0;//is ok!!!
    }
    t++;
  }while(t<0xffffff);
  return 1;
}







void SD_DisSelect(void)
{
	digitalWrite(slaveSelectPin, HIGH);
	SPIx_ReadWriteByte(0xff);
}

uint8_t SD_Select(void)
{
	digitalWrite(slaveSelectPin, LOW);
	if(SD_WaitReady()==0)
          return 0;
	SD_DisSelect();
	return 1;
}

uint8_t SD_SendCmd(uint8_t cmd, u32 arg, uint8_t crc)
{
    uint8_t r1;	
    uint8_t Retry=0; 
    SD_DisSelect();
    if(SD_Select())
    {
      return 0XFF;
    }
    SPIx_ReadWriteByte(cmd | 0x40);//¡¤?¡ÀeD¡ä¨¨??¨¹¨¢?
    SPIx_ReadWriteByte(arg >> 24);
    SPIx_ReadWriteByte(arg >> 16);
    SPIx_ReadWriteByte(arg >> 8);
    SPIx_ReadWriteByte(arg);	  
    SPIx_ReadWriteByte(crc); 
    if(cmd==CMD12)
      SPIx_ReadWriteByte(0xff);
    Retry=0X1F;
    do
    {
	r1=SPIx_ReadWriteByte(0xFF);
    }while((r1&0X80) && Retry--);	 

    return r1;
}

uint8_t SD_Initialize(void)
{
	u16 retry;
	uint8_t r1;
	uint8_t buf[4]; 
	
	// set the slaveSelectPin as an output:
	//pinMode (slaveSelectPin, OUTPUT);
	// initialize SPI:
	SPIx_Init();
       SPIx_SetSpeed(SPI_BaudRatePrescaler_256);
    //    SPI.setDataMode(SPI_MODE3);
	//SPI.setDataMode(SPI_MODE3);
	//SPI.setClockDivider(SPI_CLOCK_DIV256);
	for(int i=0;i<10;i++)
	SPIx_ReadWriteByte(0xff);
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);
	}while((r1!=0x01)&&retry--);
	SD_Type=0;
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(int i=0;i<4;i++)buf[i]=SPIx_ReadWriteByte(0XFF);	
			if(buf[2]==0X01&&buf[3]==0XAA)
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	
					r1=SD_SendCmd(CMD41,0x40000000,0X01);
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)
				{
					for(int i=0;i<4;i++)buf[i]=SPIx_ReadWriteByte(0XFF);
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;   
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);	
			r1=SD_SendCmd(CMD41,0,0X01);	
			if(r1<=1)
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do 
				{
					SD_SendCmd(CMD55,0,0X01);
					r1=SD_SendCmd(CMD41,0,0X01);
				}while(r1&&retry--);
			}else
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do 
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;
		}
	}
	SD_DisSelect();
	//SPIsetClockDivider(SPI_BaudRatePrescaler_2);
	if(SD_Type)
	return 0;
	else if(r1)return r1; 	   
	return 0xaa;
}

uint8_t SD_GetResponse(uint8_t Response)
{
	u16 Count=0xFFF;						  
	while ((SPIx_ReadWriteByte(0XFF)!=Response)&&Count)Count--;
	if (Count==0)return MSD_RESPONSE_FAILURE;
	else return MSD_RESPONSE_NO_ERROR;
}

uint8_t SD_RecvData(uint8_t*buf,u16 len)
{			  	  
    if(SD_GetResponse(0xFE))return 1;
    while(len--)
    {
        *buf=SPIx_ReadWriteByte(0xFF);
        buf++;
    }
    
    SPIx_ReadWriteByte(0xFF);
    SPIx_ReadWriteByte(0xFF);									  					    
    return 0;
}

uint8_t SD_GetCSD(uint8_t *csd_data)
{
    uint8_t r1;	 
    r1=SD_SendCmd(CMD9,0,0x01);
    if(r1==0)
    {
      r1=SD_RecvData(csd_data, 16);
    }
  SD_DisSelect();
  if(r1)
    return 1;
  else 
    return 0;
} 

u32 SD_GetSectorCount(void)
{
    uint8_t csd[16];
    u32 Capacity;  
    uint8_t n;
    u16 csize;  					    
    if(SD_GetCSD(csd)!=0) 
    {
      return 0;	    
    }

    if((csd[0]&0xC0)==0x40)	
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;	 		   
    }
    else
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9); 
    }
    return Capacity;
}

uint8_t SD_SendBlock(const uint8_t*buf,uint8_t cmd)
{	
	u16 t;		  	  
	if(SD_WaitReady())return 1;
	SPIx_ReadWriteByte(cmd);
	if(cmd!=0XFD)
	{
	    for(t=0;t<512;t++)
              SPIx_ReadWriteByte(buf[t]);
	      SPIx_ReadWriteByte(0xFF);
	      SPIx_ReadWriteByte(0xFF);
		t=SPIx_ReadWriteByte(0xFF);
		if((t&0x1F)!=0x05)return 2;								  					    
	}						 									  					    
    return 0;
}

uint8_t SD_ReadDisk(uint8_t* buf,u32 sector,uint8_t cnt)
{
	uint8_t r1;
	if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);
		if(r1==0)
		{
			r1=SD_RecvData(buf,512);
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);
		do
		{
			r1=SD_RecvData(buf,512);
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	
	}   
	SD_DisSelect();
	return r1;//
}

uint8_t SD_WriteDisk(const uint8_t* buf,u32 sector,uint8_t cnt)
{
	uint8_t r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);
		if(r1==0)
		{
			r1=SD_SendBlock(buf,0xFE);
		}
	}
        else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);
		}
	}   
	SD_DisSelect();
	return r1;//
}	 
