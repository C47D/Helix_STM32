#include "ffconf.h"
#include "ff.h"
#include "df_i2c.h"






extern int DF_Usart1Init(void);



FATFS fs;

int  bsp_init(void){

    FRESULT tmp;
    int res = 0;
   
    tmp = f_mount(&fs, "1:", 1); //�����������������ļ�ϵͳʱ����0������ʾ�����mount�� ��flash���ļ�ϵͳʱ��'1'.��ʾ����mount��
    if (tmp != FR_OK){
        
        tmp = -1;
    
    }
    //initailize the usart1
    DF_Usart1Init();    
    
   
    return res;

}
