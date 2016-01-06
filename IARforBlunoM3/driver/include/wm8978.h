#ifndef __WM8978_H
#define __WM8978_H
 									
#include "stm32f10x.h"

__packed typedef struct 
{							  
	uint8_t mvol;		//����(����������һ���),��Χ:0~63
	uint8_t cfreq[5];	//����Ƶ��ѡ������,ÿ��Ԫ�ؿ�ѡ��ΧΪ0~3,�Ҵ����Ƶ�ʸ�����ͬ 
					//[0]��ӦƵ��:80,105,135,175
					//[1]��ӦƵ��:230,300,385,500
					//[2]��ӦƵ��:650,850,1100,1400
					//[3]��ӦƵ��:1800,2400,3200,4100
					//[4]��ӦƵ��:5300,6900,9000,11700
	uint8_t freqval[5];	//����Ƶ�������
	uint8_t d3;			//3d����	
	uint8_t speakersw;	//�������ȿ���,0,�ر�;1,�� 
	uint8_t saveflag; 	//�����־,0X0A,�������;����,����δ����	   
}_wm8978_obj;

extern _wm8978_obj wm8978set;//WM8978������

//���AD0��(4��)�ӵ�,IIC��ַΪ0X4A(���������λ).
//�����V3.3,��IIC��ַΪ0X4B(���������λ).
#define WM8978_ADDR				0X1A	//WM8978��������ַ,�̶�Ϊ0X1A 
 
#define EQ1_80Hz		0X00
#define EQ1_105Hz		0X01
#define EQ1_135Hz		0X02
#define EQ1_175Hz		0X03

#define EQ2_230Hz		0X00
#define EQ2_300Hz		0X01
#define EQ2_385Hz		0X02
#define EQ2_500Hz		0X03

#define EQ3_650Hz		0X00
#define EQ3_850Hz		0X01
#define EQ3_1100Hz		0X02
#define EQ3_14000Hz		0X03

#define EQ4_1800Hz		0X00
#define EQ4_2400Hz		0X01
#define EQ4_3200Hz		0X02
#define EQ4_4100Hz		0X03

#define EQ5_5300Hz		0X00
#define EQ5_6900Hz		0X01
#define EQ5_9000Hz		0X02
#define EQ5_11700Hz		0X03

 
 
 
void wm8978_read_para(_wm8978_obj * wm8978dev);
void wm8978_save_para(_wm8978_obj * wm8978dev);

uint8_t WM8978_Init(void); 
void WM8978_ADDA_Cfg(uint8_t dacen,uint8_t adcen);
void WM8978_Input_Cfg(uint8_t micen,uint8_t lineinen,uint8_t auxen);
void WM8978_Output_Cfg(uint8_t dacen,uint8_t bpsen);
void WM8978_MIC_Gain(uint8_t gain);
void WM8978_LINEIN_Gain(uint8_t gain);
void WM8978_AUX_Gain(uint8_t gain);
uint8_t WM8978_Write_Reg(uint8_t reg,uint16_t val); 
uint16_t WM8978_Read_Reg(uint8_t reg);
void WM8978_HPvol_Set(uint8_t voll,uint8_t volr);
void WM8978_SPKvol_Set(uint8_t volx);
void WM8978_I2S_Cfg(uint8_t fmt,uint8_t len);
void WM8978_3D_Set(uint8_t depth);
void WM8978_EQ_3D_Dir(uint8_t dir); 
void WM8978_EQ1_Set(uint8_t cfreq,uint8_t gain); 
void WM8978_EQ2_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ3_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ4_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ5_Set(uint8_t cfreq,uint8_t gain);

#endif





















