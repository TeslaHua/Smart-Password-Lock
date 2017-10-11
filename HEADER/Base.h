#ifndef __BASE_H_
#define __BASE_H_
#include<reg51.h>

sbit IRIN=P3^2;    //����һ����������ݿ�

sbit Beep=P2^0;    //�����������ʱ����������
sbit led0=P2^1;	   //�����̵����ڶ˿�
sbit led1=P2^2;	   //���������ڶ˿�

#define Key P1	    //���������̵�8�������˿�


sbit DSIO=P3^4;   //���������ֱ���DS1302��I/O�ڣ���λ�˺�ʱ���źŶ�
sbit RST=P3^5;
sbit SCLK=P3^6;

//����LCD1602Ҫ�õ��Ķ˿�
#define LCD P0	    //����LCD1602���õ�8�����ݿ�
sbit E=P2^7;	   //ʹ�ܶ�
sbit RW=P2^5;	   //������
sbit RS=P2^6;	   //д����


//����AT24C02Ҫ�õ��Ķ˿�
sbit I2C_SCL = P2^3;
sbit I2C_SDA = P2^4;

#define uchar unsigned char
#define uint unsigned int

#endif