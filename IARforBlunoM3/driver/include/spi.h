#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"


 
 				  	    													  
void SPIx_Init(void);			 //��ʼ��SPI��
void SPIx_SetSpeed(uint8_t SpeedSet); //����SPI�ٶ�   
u8 SPIx_ReadWriteByte(uint8_t TxData);//SPI���߶�дһ���ֽ�
		 
#endif

