#include "df_i2s.h"  
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"






static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* SPI2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB, GPIOC and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                          RCC_APB2Periph_AFIO, ENABLE);

  /* I2S2 SD, CK and WS pins configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* I2S2 MCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}



/**
  * @brief  Configure the I2S Peripheral.
  * @param  Standard: I2S_Standard_Phillips, I2S_Standard_MSB or I2S_Standard_LSB
  * @param MCLKOutput: I2S_MCLKOutput_Enable or I2S_MCLKOutput_Disable
  * @param AudioFreq: I2S_AudioFreq_8K, I2S_AudioFreq_16K, I2S_AudioFreq_22K,
  *   I2S_AudioFreq_44K or I2S_AudioFreq_48K
  * @retval : None
  */
void I2S2_Init(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq)
{

  I2S_InitTypeDef I2S_InitStructure;


   /* Configure I2S interrupt Channel */
 // NVIC_Config();
  
  //config the GPIO pin for I2S
  GPIO_Config();
  
  /* Enable I2S2 APB1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* Deinitialize SPI2 peripheral */
  SPI_I2S_DeInit(SPI2);

  /* I2S2 peripheral configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = Standard;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = MCLKOutput;
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &I2S_InitStructure);

  /* Disable the I2S2 TXE Interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);

  /* Enable the SPI2/I2S2 peripheral */
  I2S_Cmd(SPI2, ENABLE);


} 





//I2S2ext����,����ȫ˫��ģʽ����,��Ҫʱ,��I2S2_Init֮�����
//std:I2S��׼,00,�����ֱ�׼;01,MSB�����׼(�Ҷ���);10,LSB�����׼(�����);11,PCM��׼
//mode:I2S����ģʽ,00,�ӻ�����;01,�ӻ�����;I2S2ext�����ڴӻ�ģʽ
//cpol:0,ʱ�ӵ͵�ƽ��Ч;1,ʱ�Ӹߵ�ƽ��Ч
//datalen:���ݳ���,0,16λ��׼;1,16λ��չ(frame=32bit);2,24λ;3,32λ.
void I2S2ext_Init(uint8_t std,uint8_t mode,uint8_t cpol,uint8_t datalen)
{  
	
} 

//�����ʼ��㹫ʽ:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
//I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
//һ��HSE=8Mhz 
//pllm:��Sys_Clock_Set���õ�ʱ��ȷ����һ����8
//PLLI2SN:һ����192~432 
//PLLI2SR:2~7
//I2SDIV:2~255
//ODD:0/1
//I2S��Ƶϵ����@pllm=8,HSE=8Mhz,��vco����Ƶ��Ϊ1Mhz
//���ʽ:������/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
const uint16_t I2S_PSC_TBL[][5]=
{
	{800 ,256,5,12,1},		//8Khz������
	{1102,429,4,19,0},		//11.025Khz������ 
	{1600,213,2,13,0},		//16Khz������
	{2205,429,4, 9,1},		//22.05Khz������
	{3200,213,2, 6,1},		//32Khz������
	{4410,271,2, 6,0},		//44.1Khz������
	{4800,258,3, 3,1},		//48Khz������
	{8820,316,2, 3,1},		//88.2Khz������
	{9600,344,2, 3,1},  	//96Khz������
	{17640,361,2,2,0},  	//176.4Khz������ 
	{19200,393,2,2,0},  	//192Khz������
};  
//����IIS�Ĳ�����(@MCKEN)
//samplerate:������,��λ:Hz
//����ֵ:0,���óɹ�;1,�޷�����.
uint8_t I2S2_SampleRate_Set(uint32_t samplerate)
{ 
	return 0;
}  
//I2S2 TX DMA����
//����Ϊ˫����ģʽ,������DMA��������ж�
//buf0:M0AR��ַ.
//buf1:M1AR��ַ.
//num:ÿ�δ���������
void I2S2_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num)
{  
	
} 
//I2S2ext RX DMA����
//����Ϊ˫����ģʽ,������DMA��������ж�
//buf0:M0AR��ַ.
//buf1:M1AR��ַ.
//num:ÿ�δ���������
void I2S2ext_RX_DMA_Init(uint8_t* buf0,uint8_t *buf1,uint16_t num)
{  
	
} 

//I2S DMA�ص�����ָ��
void (*i2s_tx_callback)(void);	//TX�ص�����
void (*i2s_rx_callback)(void);	//RX�ص�����

//DMA1_Stream4�жϷ�����
void DMA1_Stream4_IRQHandler(void)
{      
								 
} 
//DMA1_Stream3�жϷ�����
void DMA1_Stream3_IRQHandler(void)
{      
											 
} 
//I2S��ʼ����
void I2S_Play_Start(void)
{   	  
	//DMA1_Stream4->CR|=1<<0;		//����DMA TX����  		
}
//�ر�I2S����
void I2S_Play_Stop(void)
{   	 
	//DMA1_Stream4->CR&=~(1<<0);	//��������	 	 
}
//I2S��ʼ¼��
void I2S_Rec_Start(void)
{   	    
	//DMA1_Stream3->CR|=1<<0;		//����DMA RX���� 		
}
//�ر�I2S¼��
void I2S_Rec_Stop(void)
{   	  
	//DMA1_Stream3->CR&=~(1<<0);	//����¼��		 
}








