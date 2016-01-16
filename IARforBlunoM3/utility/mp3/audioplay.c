#include "audioplay.h"
#include "ff.h"
#include "wm8978.h"
#include "df_i2s.h"

#include "string.h"  



extern int DF_Usart1Init(void);


//音乐播放控制器
__audiodev audiodev;	  



//开始音频播放
void audio_start(void)
{
	audiodev.status=3<<0;//开始播放+非暂停
	I2S_Play_Start();
} 
//关闭音频播放
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  

//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
uint16_t audio_get_tnum(uint8_t *path)
{	  
 return 0;
}
//显示曲目索引
//index:当前索引
//total:总文件数
void audio_index_show(uint16_t index,uint16_t total)
{
		  	  
}
 
//显示播放时间,比特率 信息  
//totsec;音频文件总时间长度
//cursec:当前播放时间
//bitrate:比特率(位速)
void audio_msg_show(uint32_t totsec,uint32_t cursec,uint32_t bitrate)
{	

}
//播放音乐
uint8_t audio_play(void)
{
	 return 0;
} 
//播放某个音频文件
uint8_t audio_play_song(uint8_t* fname)
{
	 return 0;
}












