#ifndef __LedAndBuzzer_H_
#define __LedAndBuzzer_H_
#include<Base.h>


#define led_off  1    //��ʾ�ص�
#define led_on 0	 //��ʾ����
#define green 1		 //��ʾ�̵�
#define red 0		 //��ʾ���
#define buzzer_on 0  //��ʾ��������
#define buzzer_off 1  //��ʾ������ֹͣ��

void LightOrClose(uchar led, uchar state);   //��һ�ŵƵ���
void BuzzerOpenOrClose(uchar state);

					  
#endif