#include "coder.h"
#include "mp3dec.h"
#include "df_mp3play.h"
#include "ff.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"


__mp3ctrl * mp3ctrl;        //the global variable for handle the mp3 data contrl block.


void mp3_i2s_dma_tx_callback(void) {

  
}

void mp3_fill_buffer(uint16_t* buf,uint16_t size,uint8_t nch){

}

uint8_t mp3_id3v1_decode(uint8_t* buf,__mp3ctrl *pctrl){


}

uint8_t mp3_id3v2_decode(uint8_t* buf,uint32_t size,__mp3ctrl *pctrl){
    
    ID3V2_TagHead *taghead;
	ID3V23_FrameHead *framehead; 
	u32 t;
	u32 tagsize;	//tag大小
	u32 frame_size;	//帧大小 
	taghead=(ID3V2_TagHead*)buf; 
	if(strncmp("ID3",(const char*)taghead->id,3)==0)//存在ID3?
	{
		tagsize=((u32)taghead->size[0]<<21)|((u32)taghead->size[1]<<14)|((u16)taghead->size[2]<<7)|taghead->size[3];//得到tag 大小
		pctrl->datastart=tagsize;		//得到mp3数据开始的偏移量
		if(tagsize>size)tagsize=size;	//tagsize大于输入bufsize的时候,只处理输入size大小的数据
		if(taghead->mversion<3)
		{
			printf("not supported mversion!\r\n");
			return 1;
		}
		t=10;
		while(t<tagsize)
		{
			framehead=(ID3V23_FrameHead*)(buf+t);
			frame_size=((u32)framehead->size[0]<<24)|((u32)framehead->size[1]<<16)|((u32)framehead->size[2]<<8)|framehead->size[3];//得到帧大小
 			if (strncmp("TT2",(char*)framehead->id,3)==0||strncmp("TIT2",(char*)framehead->id,4)==0)//找到歌曲标题帧,不支持unicode格式!!
			{
				strncpy((char*)pctrl->title,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_TITSIZE_MAX-1));
			}
 			if (strncmp("TP1",(char*)framehead->id,3)==0||strncmp("TPE1",(char*)framehead->id,4)==0)//找到歌曲艺术家帧
			{
				strncpy((char*)pctrl->artist,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_ARTSIZE_MAX-1));
			}
			t+=frame_size+sizeof(ID3V23_FrameHead);
		} 
	}else pctrl->datastart=0;//不存在ID3,mp3数据是从0开始
	return 0;


}

uint8_t mp3_get_info(uint8_t *pname,__mp3ctrl* pctrl){

    HMP3Decoder decoder;
    MP3FrameInfo frame_info;
	MP3_FrameXing* fxing;
	MP3_FrameVBRI* fvbri;
	FIL*fmp3;
	u8 *buf;
	u32 br;
	u8 res;
	int offset=0;
	u32 p;
	short samples_per_frame;	//一帧的采样个数
	u32 totframes;				//总帧数
	
	fmp3= (FIL*)malloc(sizeof(FIL)); 
	buf= (u8 *)malloc(5*1024);		//申请5K内存 
	if(fmp3&&buf)//内存申请成功
	{ 		
		f_open(fmp3,(const TCHAR*)pname,FA_READ);//打开文件
		res=f_read(fmp3,(char*)buf,5*1024,&br);
		if(res==0)//读取文件成功,开始解析ID3V2/ID3V1以及获取MP3信息
		{  
			mp3_id3v2_decode(buf,br,pctrl);	//解析ID3V2数据
			f_lseek(fmp3,fmp3->fsize-128);	//偏移到倒数128的位置
			f_read(fmp3,(char*)buf,128,&br);//读取128字节
			mp3_id3v1_decode(buf,pctrl);	//解析ID3V1数据  
			decoder=MP3InitDecoder(); 		//MP3解码申请内存
			f_lseek(fmp3,pctrl->datastart);	//偏移到数据开始的地方
			f_read(fmp3,(char*)buf,5*1024,&br);	//读取5K字节mp3数据
 			offset=MP3FindSyncWord(buf,br);	//查找帧同步信息
			if(offset>=0&&MP3GetNextFrameInfo(decoder,&frame_info,&buf[offset])==0)//找到帧同步信息了,且下一阵信息获取正常	
			{ 
				p=offset+4+32;
				fvbri=(MP3_FrameVBRI*)(buf+p);
				if(strncmp("VBRI",(char*)fvbri->id,4)==0)//存在VBRI帧(VBR格式)
				{
					if (frame_info.version==MPEG1)samples_per_frame=1152;//MPEG1,layer3每帧采样数等于1152
					else samples_per_frame=576;//MPEG2/MPEG2.5,layer3每帧采样数等于576 
 					totframes=((u32)fvbri->frames[0]<<24)|((u32)fvbri->frames[1]<<16)|((u16)fvbri->frames[2]<<8)|fvbri->frames[3];//得到总帧数
					pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//得到文件总长度
				}else	//不是VBRI帧,尝试是不是Xing帧(VBR格式)
				{  
					if (frame_info.version==MPEG1)	//MPEG1 
					{
						p=frame_info.nChans==2?32:17;
						samples_per_frame = 1152;	//MPEG1,layer3每帧采样数等于1152
					}else
					{
						p=frame_info.nChans==2?17:9;
						samples_per_frame=576;		//MPEG2/MPEG2.5,layer3每帧采样数等于576
					}
					p+=offset+4;
					fxing=(MP3_FrameXing*)(buf+p);
					if(strncmp("Xing",(char*)fxing->id,4)==0||strncmp("Info",(char*)fxing->id,4)==0)//是Xng帧
					{
						if(fxing->flags[3]&0X01)//存在总frame字段
						{
							totframes=((u32)fxing->frames[0]<<24)|((u32)fxing->frames[1]<<16)|((u16)fxing->frames[2]<<8)|fxing->frames[3];//得到总帧数
							pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//得到文件总长度
						}else	//不存在总frames字段
						{
							pctrl->totsec=fmp3->fsize/(frame_info.bitrate/8);
						} 
					}else 		//CBR格式,直接计算总播放时间
					{
						pctrl->totsec=fmp3->fsize/(frame_info.bitrate/8);
					}
				} 
				pctrl->bitrate=frame_info.bitrate;			//得到当前帧的码率
				mp3ctrl->samplerate=frame_info.samprate; 	//得到采样率. 
				if(frame_info.nChans==2)mp3ctrl->outsamples=frame_info.outputSamps; //输出PCM数据量大小 
				else mp3ctrl->outsamples=frame_info.outputSamps*2; //输出PCM数据量大小,对于单声道MP3,直接*2,补齐为双声道输出
			}else res=0XFE;//未找到同步帧	
			MP3FreeDecoder(decoder);//释放内存		
		} 
		f_close(fmp3);
	}else res=0XFF;
	myfree(fmp3);
	myfree(buf);	
	return res;	

  
  
}
uint8_t mp3_play_song(uint8_t* fname){
  
    
  
  
  
  
}




