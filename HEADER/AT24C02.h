#ifndef __AT24C02_H_
#define __AT24C02_H_

#include<Base.h>
#include<LCD1602.h>

void I2C_Delay10us();   //IIC总线要用到的短延时
void I2C_Start();		//IIC启动信号
void I2C_Stop();		//IIC停止信号
uchar I2C_SendByte(uchar dat, uchar ack); //IIC发送一个字节
uchar I2C_ReadByte();  //IIC读取一个字节
void At24c02Write(uchar addr,uchar *Ptr); //At24c02连续写入8个字节数据
void At24c02Read(uchar addr,uchar *Ptr);  //At24c02连续读取8个字节数据

void SerialInit();	    //串口初始化
void SendToPc(uchar dat); //串口发送一个字节到上位机

#endif