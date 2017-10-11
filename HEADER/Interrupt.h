#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include"LedAndBuzzer.h"
#include"LCD1602.h"
#include"DS1302.h"

uint T_count=0;	                                     //T_count表示定时器计数个数

extern uchar count;
extern uchar Compare_flag,First_flag,AllowModify_flag,Second_flag; //Compare_flag的作用是标志着可以进行比较输入的密码是否正确了;finish_flag作用控制显示输入新密码的标志，就是CDIS1和CDIS2;
extern uchar Timer_flag;                       //AllowModify_flag的作用是要修改密码时的标志位(只有输对密码后才能修改密码),First_flag表示第一次设置密码完成标志，Second_flag表示第二次。
uchar Modify_flag=0;                          //Modify_flag的作用是修改密码，并把密码覆盖掉原来的密码标志
uchar Value[4];        //用来接收遥控器发来的4个数据（存放过程中产生的地址码,地址反码,命令码和命令反码）
extern uchar Value1[]; //正确密码 ，也用来装载新密码  12345678
extern uchar Value2[]; //用来接收用户输入的密码以便和原始密码比较
extern uchar Value3[];
uchar code CDIS8[]={"  COMFORM YOUR  "};  //提示用户确认密码
uchar code CDIS9[]={"CODE:  -------- "};  //用户再一次输入要设定的密码
extern uchar code CDIS12[];	//提示用户密码设置失败
extern uchar code CDIS13[];  
uchar num2=0;                          //记录输入的密码数


uchar delay_count=0;  

uchar code CDIS6[]={" PLEASE SET NEW "};  //提醒修改密码
uchar code CDIS7[]={"CODE:  -------- "};
extern uchar code CDIS1[];  //提醒输入密码
extern uchar code CDIS2[];
extern uchar code CDIS4[];  //提醒输入密码
extern uchar code CDIS5[]; 


extern void Display(uchar CDIS1[],uchar CDIS2[]); //显示在LCD1602上的具体内容
void Input_Password();            //4位密码的存入一个数组（无论新老密码）
uchar Judge_password(uchar value[]); //判断输入的密码是否在0到9之间的有效密码	
void  Shown_Asterisks(uchar num2);  //用户输入密码显示星号
				  
#endif


