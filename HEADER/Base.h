#ifndef __BASE_H_
#define __BASE_H_
#include<reg51.h>

sbit IRIN=P3^2;    //定义一个红外的数据口

sbit Beep=P2^0;    //三次输入错误时密码锁报警
sbit led0=P2^1;	   //定义绿灯所在端口
sbit led1=P2^2;	   //定义红灯所在端口

#define Key P1	    //定义矩阵键盘的8个按键端口


sbit DSIO=P3^4;   //以下三个分别是DS1302的I/O口，复位端和时钟信号端
sbit RST=P3^5;
sbit SCLK=P3^6;

//定义LCD1602要用到的端口
#define LCD P0	    //定义LCD1602所用的8个数据口
sbit E=P2^7;	   //使能端
sbit RW=P2^5;	   //读数据
sbit RS=P2^6;	   //写数据


//定义AT24C02要用到的端口
sbit I2C_SCL = P2^3;
sbit I2C_SDA = P2^4;

#define uchar unsigned char
#define uint unsigned int

#endif