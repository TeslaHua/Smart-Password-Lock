#include"LCD1602.h"


/*
功能描述：ms级延时函数
返回值：无
参数：外层循环次数
*/
void Delayms(uchar c)   //延时在1~2毫秒	   
{
    uchar a,b;
	for (a=c; a>0; a--) 
		for(b=15;b>0;b--);   	
}

/*
功能描述：写命令
返回值：无
参数：8位的命令数据
*/
void lcdwritecom(uchar com)	            //写入命令
{
	E = 0;	 //使能清零
	RS = 0;	 //选择写入命令
	LCD = com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Delayms(40);
	E=1;
	Delayms(40);
	E=0;
}	

/*
功能描述：写数据
返回值：无
参数：8位的数据
*/   
void lcdwritedate(uchar dat)			//写入数据
{
	E = 0;	 //使能清零
	RS = 1;	 //选择写入命令	
	LCD = dat;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Delayms(40);
	E=1;
	Delayms(40);
	E=0;
} 

/*
功能描述：LCD1602初始化
返回值：无
参数：无
*/  
void lcdinit()		    //LCD初始化子程序
{	
	RW = 0;
 	lcdwritecom(0x38);  //显示模式设置，16*2显示，5*7点阵，8位数据接口
	lcdwritecom(0x0C);  //开显示，不显示光标，光标不移动
	lcdwritecom(0x06);  //写一个指针加1
	lcdwritecom(0x01);  //清屏
}
