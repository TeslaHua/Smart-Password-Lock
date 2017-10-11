#ifndef __AT24C02_H_
#define __AT24C02_H_

#include<Base.h>
#include<LCD1602.h>

void I2C_Delay10us();   //IIC����Ҫ�õ��Ķ���ʱ
void I2C_Start();		//IIC�����ź�
void I2C_Stop();		//IICֹͣ�ź�
uchar I2C_SendByte(uchar dat, uchar ack); //IIC����һ���ֽ�
uchar I2C_ReadByte();  //IIC��ȡһ���ֽ�
void At24c02Write(uchar addr,uchar *Ptr); //At24c02����д��8���ֽ�����
void At24c02Read(uchar addr,uchar *Ptr);  //At24c02������ȡ8���ֽ�����

void SerialInit();	    //���ڳ�ʼ��
void SendToPc(uchar dat); //���ڷ���һ���ֽڵ���λ��

#endif