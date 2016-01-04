#ifndef __SD_H
#define __SD_H
#include "stm32f10x.h"







uint8_t SD_Initialize(void);
uint8_t SD_WriteDisk(const uint8_t* buf,u32 sector,uint8_t cnt);
uint8_t SD_ReadDisk(uint8_t* buf,u32 sector,uint8_t cnt);
u32 SD_GetSectorCount(void); //返回扇区数 每一个扇区有512个字节.












#endif