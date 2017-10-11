#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include"LedAndBuzzer.h"
#include"LCD1602.h"
#include"DS1302.h"

uint T_count=0;	                                     //T_count��ʾ��ʱ����������

extern uchar count;
extern uchar Compare_flag,First_flag,AllowModify_flag,Second_flag; //Compare_flag�������Ǳ�־�ſ��Խ��бȽ�����������Ƿ���ȷ��;finish_flag���ÿ�����ʾ����������ı�־������CDIS1��CDIS2;
extern uchar Timer_flag;                       //AllowModify_flag��������Ҫ�޸�����ʱ�ı�־λ(ֻ��������������޸�����),First_flag��ʾ��һ������������ɱ�־��Second_flag��ʾ�ڶ��Ρ�
uchar Modify_flag=0;                          //Modify_flag���������޸����룬�������븲�ǵ�ԭ���������־
uchar Value[4];        //��������ң����������4�����ݣ���Ź����в����ĵ�ַ��,��ַ����,�����������룩
extern uchar Value1[]; //��ȷ���� ��Ҳ����װ��������  12345678
extern uchar Value2[]; //���������û�����������Ա��ԭʼ����Ƚ�
extern uchar Value3[];
uchar code CDIS8[]={"  COMFORM YOUR  "};  //��ʾ�û�ȷ������
uchar code CDIS9[]={"CODE:  -------- "};  //�û���һ������Ҫ�趨������
extern uchar code CDIS12[];	//��ʾ�û���������ʧ��
extern uchar code CDIS13[];  
uchar num2=0;                          //��¼�����������


uchar delay_count=0;  

uchar code CDIS6[]={" PLEASE SET NEW "};  //�����޸�����
uchar code CDIS7[]={"CODE:  -------- "};
extern uchar code CDIS1[];  //������������
extern uchar code CDIS2[];
extern uchar code CDIS4[];  //������������
extern uchar code CDIS5[]; 


extern void Display(uchar CDIS1[],uchar CDIS2[]); //��ʾ��LCD1602�ϵľ�������
void Input_Password();            //4λ����Ĵ���һ�����飨�����������룩
uchar Judge_password(uchar value[]); //�ж�����������Ƿ���0��9֮�����Ч����	
void  Shown_Asterisks(uchar num2);  //�û�����������ʾ�Ǻ�
				  
#endif


