#ifndef __DF_I2C_H
#define __DF_I2C_H

#include "stm32f10x_gpio.h"
#include "stdbool.h"

#define SDA_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define SDA_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define SDA_READ()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)

#define SCL_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define SCL_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define I2C_DELAY() do{for(int i=0;i<100 ;i++) {asm volatile("nop");}}while(0)

   	   	
//IIC���в�������

void DF_IIC_Init(void);                //��ʼ��IIC��IO��				 
void DF_IIC_Start(void);				//����IIC��ʼ�ź�
void DF_IIC_Stop(void);	  			//����IICֹͣ�ź�
void DF_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t DF_IIC_Read_Byte();//IIC��ȡһ���ֽ�
bool DF_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void DF_IIC_Ack(void);					//IIC����ACK�ź�
void DF_IIC_NAck(void);				//IIC������ACK�ź� 
#endif


