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
	u32 tagsize;	//tag��С
	u32 frame_size;	//֡��С 
	taghead=(ID3V2_TagHead*)buf; 
	if(strncmp("ID3",(const char*)taghead->id,3)==0)//����ID3?
	{
		tagsize=((u32)taghead->size[0]<<21)|((u32)taghead->size[1]<<14)|((u16)taghead->size[2]<<7)|taghead->size[3];//�õ�tag ��С
		pctrl->datastart=tagsize;		//�õ�mp3���ݿ�ʼ��ƫ����
		if(tagsize>size)tagsize=size;	//tagsize��������bufsize��ʱ��,ֻ��������size��С������
		if(taghead->mversion<3)
		{
			printf("not supported mversion!\r\n");
			return 1;
		}
		t=10;
		while(t<tagsize)
		{
			framehead=(ID3V23_FrameHead*)(buf+t);
			frame_size=((u32)framehead->size[0]<<24)|((u32)framehead->size[1]<<16)|((u32)framehead->size[2]<<8)|framehead->size[3];//�õ�֡��С
 			if (strncmp("TT2",(char*)framehead->id,3)==0||strncmp("TIT2",(char*)framehead->id,4)==0)//�ҵ���������֡,��֧��unicode��ʽ!!
			{
				strncpy((char*)pctrl->title,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_TITSIZE_MAX-1));
			}
 			if (strncmp("TP1",(char*)framehead->id,3)==0||strncmp("TPE1",(char*)framehead->id,4)==0)//�ҵ�����������֡
			{
				strncpy((char*)pctrl->artist,(char*)(buf+t+sizeof(ID3V23_FrameHead)+1),AUDIO_MIN(frame_size-1,MP3_ARTSIZE_MAX-1));
			}
			t+=frame_size+sizeof(ID3V23_FrameHead);
		} 
	}else pctrl->datastart=0;//������ID3,mp3�����Ǵ�0��ʼ
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
	short samples_per_frame;	//һ֡�Ĳ�������
	u32 totframes;				//��֡��
	
	fmp3= (FIL*)malloc(sizeof(FIL)); 
	buf= (u8 *)malloc(5*1024);		//����5K�ڴ� 
	if(fmp3&&buf)//�ڴ�����ɹ�
	{ 		
		f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�
		res=f_read(fmp3,(char*)buf,5*1024,&br);
		if(res==0)//��ȡ�ļ��ɹ�,��ʼ����ID3V2/ID3V1�Լ���ȡMP3��Ϣ
		{  
			mp3_id3v2_decode(buf,br,pctrl);	//����ID3V2����
			f_lseek(fmp3,fmp3->fsize-128);	//ƫ�Ƶ�����128��λ��
			f_read(fmp3,(char*)buf,128,&br);//��ȡ128�ֽ�
			mp3_id3v1_decode(buf,pctrl);	//����ID3V1����  
			decoder=MP3InitDecoder(); 		//MP3���������ڴ�
			f_lseek(fmp3,pctrl->datastart);	//ƫ�Ƶ����ݿ�ʼ�ĵط�
			f_read(fmp3,(char*)buf,5*1024,&br);	//��ȡ5K�ֽ�mp3����
 			offset=MP3FindSyncWord(buf,br);	//����֡ͬ����Ϣ
			if(offset>=0&&MP3GetNextFrameInfo(decoder,&frame_info,&buf[offset])==0)//�ҵ�֡ͬ����Ϣ��,����һ����Ϣ��ȡ����	
			{ 
				p=offset+4+32;
				fvbri=(MP3_FrameVBRI*)(buf+p);
				if(strncmp("VBRI",(char*)fvbri->id,4)==0)//����VBRI֡(VBR��ʽ)
				{
					if (frame_info.version==MPEG1)samples_per_frame=1152;//MPEG1,layer3ÿ֡����������1152
					else samples_per_frame=576;//MPEG2/MPEG2.5,layer3ÿ֡����������576 
 					totframes=((u32)fvbri->frames[0]<<24)|((u32)fvbri->frames[1]<<16)|((u16)fvbri->frames[2]<<8)|fvbri->frames[3];//�õ���֡��
					pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//�õ��ļ��ܳ���
				}else	//����VBRI֡,�����ǲ���Xing֡(VBR��ʽ)
				{  
					if (frame_info.version==MPEG1)	//MPEG1 
					{
						p=frame_info.nChans==2?32:17;
						samples_per_frame = 1152;	//MPEG1,layer3ÿ֡����������1152
					}else
					{
						p=frame_info.nChans==2?17:9;
						samples_per_frame=576;		//MPEG2/MPEG2.5,layer3ÿ֡����������576
					}
					p+=offset+4;
					fxing=(MP3_FrameXing*)(buf+p);
					if(strncmp("Xing",(char*)fxing->id,4)==0||strncmp("Info",(char*)fxing->id,4)==0)//��Xng֡
					{
						if(fxing->flags[3]&0X01)//������frame�ֶ�
						{
							totframes=((u32)fxing->frames[0]<<24)|((u32)fxing->frames[1]<<16)|((u16)fxing->frames[2]<<8)|fxing->frames[3];//�õ���֡��
							pctrl->totsec=totframes*samples_per_frame/frame_info.samprate;//�õ��ļ��ܳ���
						}else	//��������frames�ֶ�
						{
							pctrl->totsec=fmp3->fsize/(frame_info.bitrate/8);
						} 
					}else 		//CBR��ʽ,ֱ�Ӽ����ܲ���ʱ��
					{
						pctrl->totsec=fmp3->fsize/(frame_info.bitrate/8);
					}
				} 
				pctrl->bitrate=frame_info.bitrate;			//�õ���ǰ֡������
				mp3ctrl->samplerate=frame_info.samprate; 	//�õ�������. 
				if(frame_info.nChans==2)mp3ctrl->outsamples=frame_info.outputSamps; //���PCM��������С 
				else mp3ctrl->outsamples=frame_info.outputSamps*2; //���PCM��������С,���ڵ�����MP3,ֱ��*2,����Ϊ˫�������
			}else res=0XFE;//δ�ҵ�ͬ��֡	
			MP3FreeDecoder(decoder);//�ͷ��ڴ�		
		} 
		f_close(fmp3);
	}else res=0XFF;
	myfree(fmp3);
	myfree(buf);	
	return res;	

  
  
}
uint8_t mp3_play_song(uint8_t* fname){
  
    
  
  
  
  
}




