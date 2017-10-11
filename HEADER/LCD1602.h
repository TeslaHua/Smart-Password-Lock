#ifndef __LCD1602_H_
#define __LCD1602_H_
#include<Base.h>

void Delayms(uchar c);        //ms级延时函数
void lcdwritecom(uchar com);  //写命令	
void lcdwritedate(uchar dat); //写数据
void lcdinit();	              //LCD1602初始化
					  
#endif