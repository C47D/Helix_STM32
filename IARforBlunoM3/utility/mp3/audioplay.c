#include "audioplay.h"
#include "ff.h"
#include "wm8978.h"
#include "df_i2s.h"

#include "string.h"  



extern int DF_Usart1Init(void);


//���ֲ��ſ�����
__audiodev audiodev;	  



//��ʼ��Ƶ����
void audio_start(void)
{
	audiodev.status=3<<0;//��ʼ����+����ͣ
	I2S_Play_Start();
} 
//�ر���Ƶ����
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
uint16_t audio_get_tnum(uint8_t *path)
{	  
 return 0;
}
//��ʾ��Ŀ����
//index:��ǰ����
//total:���ļ���
void audio_index_show(uint16_t index,uint16_t total)
{
		  	  
}
 
//��ʾ����ʱ��,������ ��Ϣ  
//totsec;��Ƶ�ļ���ʱ�䳤��
//cursec:��ǰ����ʱ��
//bitrate:������(λ��)
void audio_msg_show(uint32_t totsec,uint32_t cursec,uint32_t bitrate)
{	

}
//��������
uint8_t audio_play(void)
{
	 return 0;
} 
//����ĳ����Ƶ�ļ�
uint8_t audio_play_song(uint8_t* fname)
{
	 return 0;
}












