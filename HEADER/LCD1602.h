#ifndef __LCD1602_H_
#define __LCD1602_H_
#include<Base.h>

void Delayms(uchar c);        //ms����ʱ����
void lcdwritecom(uchar com);  //д����	
void lcdwritedate(uchar dat); //д����
void lcdinit();	              //LCD1602��ʼ��
					  
#endif