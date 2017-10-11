#ifndef __DS1302_H_
#define __DS1302_H_

#include<Base.h>
#include<LCD1602.h>
#include<intrins.h>


void Delay1302(uint z);	  //DS1302上的延时
void Ds1302init();	  //DS1302的初始化
void Write1602sfm(uchar add,uchar date);     //1602刷新时分秒
void Write1602nyr(uchar add,uchar date);		//1602刷新年月日
void Write1302(uchar addr,uchar dat);	  //往DS1302里面的一个地址写入一个数据
uchar Read1302(uchar addr);	   //从DS1302的里面的一个地址里面读取一个数据
void Transformation();			//把从DS1302中读取的数装换为需要的数给LCD1602显示


#endif