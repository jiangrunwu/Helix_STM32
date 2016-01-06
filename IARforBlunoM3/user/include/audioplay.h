#ifndef __AUDIOPLAY_H
#define __AUDIOPLAY_H

#include "ff.h"
#include "stm32f10x.h"


extern uint8_t*const AUDIO_BTN_PIC_TBL[2][5];  	//5个图片按钮的路径
extern uint8_t*const AUDIO_BACK_PIC[3];			//三个背景图片
extern uint8_t*const AUDIO_PLAYR_PIC;			//播放 松开
extern uint8_t*const AUDIO_PLAYP_PIC;			//播放 按下
extern uint8_t*const AUDIO_PAUSER_PIC;			//暂停 松开
extern uint8_t*const AUDIO_PAUSEP_PIC;			//暂停 按下
////////////////////////////////////////////////////////////////////////////////////////////
#define AUDIO_TITLE_COLOR   	0XFFFF		//播放器标题颜色	
#define AUDIO_TITLE_BKCOLOR   	0X0000		//播放器标题背景色	

#define AUDIO_INFO_COLOR   		0X8410		//信息字体的颜色	
#define AUDIO_MAIN_BKCOLOR   	0X18E3		//主背景色	
#define AUDIO_BTN_BKCOLOR   	0XDF9F		//5个控制按钮背景色	
/////////////////////////////////////////////
#define AUDIO_LRC_MCOLOR       0XF810//0XFFE0		//当前歌词颜色为黄色
#define AUDIO_LRC_SCOLOR       0XFFFF//0X07FF		//前一句和后一句歌词颜色为青色			    
 
//音乐播放操作结果定义
typedef enum {
	AP_OK=0X00,				//正常播放完成
	AP_NEXT,				//播放下一曲
	AP_PREV,				//播放上一曲
	AP_ERR=0X80,			//播放有错误(没定义错误代码,仅仅表示出错) 
}APRESULT;

//音乐播放控制器
typedef __packed struct
{  
	//2个I2S解码的BUF
	uint8_t *i2sbuf1;
	uint8_t *i2sbuf2; 
	uint8_t *tbuf;			//零时数组
	FIL *file;			//音频文件指针 	
	uint32_t(*file_seek)(uint32_t);//文件快进快退函数 
	vu8 status;			//bit0:0,暂停播放;1,继续播放
						//bit1:0,结束播放;1,开启播放  
						//bit2~3:保留
						//bit4:0,无音乐播放;1,音乐播放中 (对外标记)		
						//bit5:0,无动作;1,执行了一次切歌操作(对外标记)
						//bit6:0,无动作;1,请求终止播放(但是不删除音频播放任务),处理完成后,播放任务自动清零该位
 						//bit7:0,音频播放任务已删除/请求删除;1,音频播放任务正在运行(允许继续执行)
	uint8_t mode;			//播放模式
						//0,全部循环;1,单曲循环;2,随机播放;
	uint8_t *path;			//当前文件夹路径
	uint8_t *name;			//当前播放的MP3歌曲名字
	uint16_t namelen;		//name所占的点数.
	uint16_t curnamepos;		//当前的偏移
    uint32_t totsec ;		//整首歌时长,单位:秒
    uint32_t cursec ;		//当前播放时长 
    uint32_t bitrate;	   	//比特率(位速)
	uint32_t samplerate;		//采样率 
	uint16_t bps;			//位数,比如16bit,24bit,32bit
	
	uint16_t curindex;		//当前播放的音频文件索引
	uint16_t mfilenum;		//音乐文件数目	    
	uint16_t *mfindextbl;	//音频文件索引表
	
}__audiodev; 
extern __audiodev audiodev;	//音乐播放控制器

//取2个值里面的较小值.
#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif

void audio_start(void);
void audio_stop(void); 

void music_play_task(void *pdata);
void audio_stop_req(__audiodev *audiodevx);
uint8_t audio_filelist(__audiodev *audiodevx);
void audio_load_ui(void);
void audio_show_vol(uint8_t pctx);
void audio_time_show(uint16_t sx,uint16_t sy,uint16_t sec); 

uint8_t audio_task_creat(void);
void audio_task_delete(void);
uint8_t audio_play(void);	    

#endif



