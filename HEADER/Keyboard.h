#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_
#include<Base.h>
#include<LCD1602.h>
#include<LedAndBuzzer.h>

extern uchar Compare_flag;   //判断用户是否输完密码（无论门内门外）
extern uchar Value2[];       //用来接收用户输入的密码以便和原始密码比较

void Key_scan();               //扫描按键值
void Judge_Value(uchar n);     //数码管显示函数
void IdentifiCol();            //确认按键所在列
void IdentifiRow(uchar m);     //确认按键所在行
uchar ConvertToCode(uchar n);  //处理门外的按键信息，并返回处理结果
extern void  Shown_Asterisks(uchar num2);  //用户输入密码显示星号
extern void Display(uchar CDIS1[],uchar CDIS2[]); //显示在LCD1602上的具体内容


extern uchar code CDIS1[];
extern uchar code CDIS2[];



#endif


