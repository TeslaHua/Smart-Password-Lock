#ifndef __DS1302_H_
#define __DS1302_H_

#include<Base.h>
#include<LCD1602.h>
#include<intrins.h>


void Delay1302(uint z);	  //DS1302�ϵ���ʱ
void Ds1302init();	  //DS1302�ĳ�ʼ��
void Write1602sfm(uchar add,uchar date);     //1602ˢ��ʱ����
void Write1602nyr(uchar add,uchar date);		//1602ˢ��������
void Write1302(uchar addr,uchar dat);	  //��DS1302�����һ����ַд��һ������
uchar Read1302(uchar addr);	   //��DS1302�������һ����ַ�����ȡһ������
void Transformation();			//�Ѵ�DS1302�ж�ȡ����װ��Ϊ��Ҫ������LCD1602��ʾ


#endif