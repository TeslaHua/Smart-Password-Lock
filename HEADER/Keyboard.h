#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_
#include<Base.h>
#include<LCD1602.h>
#include<LedAndBuzzer.h>

extern uchar Compare_flag;   //�ж��û��Ƿ��������루�����������⣩
extern uchar Value2[];       //���������û�����������Ա��ԭʼ����Ƚ�

void Key_scan();               //ɨ�谴��ֵ
void Judge_Value(uchar n);     //�������ʾ����
void IdentifiCol();            //ȷ�ϰ���������
void IdentifiRow(uchar m);     //ȷ�ϰ���������
uchar ConvertToCode(uchar n);  //��������İ�����Ϣ�������ش�����
extern void  Shown_Asterisks(uchar num2);  //�û�����������ʾ�Ǻ�
extern void Display(uchar CDIS1[],uchar CDIS2[]); //��ʾ��LCD1602�ϵľ�������


extern uchar code CDIS1[];
extern uchar code CDIS2[];



#endif


