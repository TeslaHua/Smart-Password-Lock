/*************************************************************************************************************
代码功能：本密码锁分为门外门内两部分。在没人的用户权限高，采用红外遥控器输入，均是8位密码，0~9的数字密码有效，
		  门内用户可以输入密码开锁，输入密码错误小于三次时，红灯闪烁两秒，当密码输入三次错误时，蜂鸣器报警5s，密码锁
		  锁住，门内用户无法输入密码。当用户输对密码后，（只有输入密码正确后才有此权限）可以按（0x46）键设置新密码，
		  当输入的密码够8位时按下（EQ）键再次确认输入的新密码，若两次输入的密码相同则新密码设置成功，否则失败。而且
		  当用户输入的新密码不够8位就按下（EQ）键时，系统直接提示新密码设置错误。（成功与失败都是绿灯亮，蜂鸣器报警）
		  门内的用户还有时间功能，用户按下（0x47）键时，显示屏显示当前的时间。还支持一键开锁功能，当用户输入（0x45）键时
		  系统自动开锁，5s后恢复原始状态。门外的用户的权限相对简单，利用矩阵键盘输入8位密码，第11个键表示清除之前输入的
		  一位密码，若一位密码都没输入就按下此键，系统提示用户还没有输入密码；第12个键表示密码输入完成，当用户输入的密码
		  位数不够就按下此键时，系统提示用户输入的密码位数不够。

时间：2015/11/19
*******************************************************************************************************************/


#include"Base.h"
#include"LCD1602.h"
#include"LedAndBuzzer.h"
#include"Keyboard.h"
#include"At24c02.h"
#include"DS1302.h"

uchar Compare_flag=0,finish_flag=0,AllowModify_flag=0; //Compare_flag的作用是标志着可以进行比较输入的密码是否正确了;finish_flag作用控制显示输入新密码的标志，就是CDIS1和CDIS2;
                                                                               //AllowModify_flag的作用是要修改密码时的标志位(只有输对密码后才能修改密码)
uchar Timer_flag=0;                                    //此标志位的作用用来区分是密码正确后触发的定时中断（0），还是密码错误小于三次触发的中断（2），又或者是密码错误
                                                       //等于三次而触发的中断（1）
uchar First_flag=0;	   //第一次设置新密码的标志
uchar Second_flag=0;   //第二次设置新密码的标志

uchar Value1[8]; //原始密码：12345678（设置新密码后要把此处清零重新下载）
//={0x0c,0x18,0x5e,0x08,0x1c,0x5a,0x42,0x52}
uchar Value2[9];//前八位用来接收用户输入的密码以便和原始密码比较，最后一位用来接收确认按键值
uchar Value3[9];//前八位用来存放用户新设定的密码的临时数组，最后一位用来接收确认按键值

uchar count=0;      //存放错误的次数  
uchar set_count=0;  //记录从临时空间移入密码存储空间的个数

uchar code CDIS1[]={"PLEASE ENTER THE"};  //提醒输入密码
uchar code CDIS2[]={"CODE:  -------- "};
uchar code CDIS3[]={"SORRY , AGAIN  !"};  //提醒密码错误，重新输入
uchar code CDIS4[]={" CODE IS RIGHT !"};  //提醒密码正确
uchar code CDIS5[]={" OPEN  SESAME. !"};

uchar code CDIS10[]={"  PASSWORD SET  "};   //提示用户密码设置成功
uchar code CDIS11[]={"  SUCCESSFULLY !"};
uchar code CDIS12[]={"  PASSWORD SET  "};	//提示用户密码设置失败
uchar code CDIS13[]={"     FAILED    !"};  
     
void Judge_RightOrWrong();	    //判断用户输入的密码正误
void Init();                    //红外等初始化声明
void Display(uchar CDIS1[],uchar CDIS2[]); //显示在LCD1602上的具体内容

/*
功能描述：程序主函数入口
返回值：无
参数：无
*/
void main()
{
	uchar i=0;
	Init();      //红外以及中断初始化	
	lcdinit();   //1602初始化
	Ds1302init();//DS1302初始化函数
	Display(CDIS1,CDIS2); //LCD1602的固定显示部分
	//SerialInit();       //串口初始化
	while(1)
	{		
		Key_scan();
		if(First_flag==1)         //说明第一次新密码已经设定好了
		{
			if(Second_flag==1)    //说明第二次新密码已经设定好了
			{
				EX0=0;   //关闭外部中断0，防止乱入
				if(Value2[0]==Value3[0]&&Value2[1]==Value3[1]&&Value2[2]==Value3[2]&&Value2[3]==Value3[3]
					&&Value2[4]==Value3[4]&&Value2[5]==Value3[5]&&Value2[6]==Value3[6]&&Value2[7]==Value3[7]) //两次密码进行比较
				{
					Display(CDIS10,CDIS11); //LCD1602的固定显示部分,提示用户新密码设定成功
					//以下代码为往AT24C02的初始地址为0的连续8个空间内存用户成功修改之后的新的8位密码并覆盖掉原来的

					At24c02Write(0,Value2);

					//以上代码为往AT24C02的初始地址为0的连续8个空间内存用户成功修改之后的新的8位密码并覆盖掉原来的
				}
				else
					Display(CDIS12,CDIS13); //LCD1602的固定显示部分,提示用户新密码设定失败

				First_flag=0;
				Second_flag=0;	//表示两次设置密码都已经完成，返回正常

				Timer_flag=0;  //和输对密码的效果一样，蜂鸣器提示两秒
				BuzzerOpenOrClose(buzzer_on);
				TR0=1;
			}					
		}
		Judge_RightOrWrong();	     //判断用户输入的密码正误		
	}
}

/*
功能描述：初始化函数，包括定时器0中断和外部中断0
返回值：无
参数：无
*/									 
void Init()
{
	//以下代码为往AT24C02的初始地址为0的连续8个空间内存储原始8位密码（第一边下载程序时保留，第二次下载时要删除此段代码）

	//At24c02Write(0,Value1);

	//以上代码为往AT24C02的初始地址为0的连续8个空间内存储原始8位密码（第一边下载程序时保留，第二次下载时要删除此段代码）

                      LightOrClose(green,led_off);    //初始化让绿灯灭
	LightOrClose(red,led_off);	    //初始化让红灯灭
	IT0=1; //下降沿触发
	EX0=1; //打开中断0允许
	IRIN=1;//初始化红外端口

	//定时器0主要功能是反应输入密码的正误的反馈
	TMOD=0X01;//定时器0选择方式1
	TH0=0X0FA;  //初值64000.计数1536个数
	TL0=0X00;
	ET0=1;  //开定时器0中断
	

	EA=1;  //打开总中断
}

/*
功能描述：判断用户输入的密码正误，并作出相应的反馈
返回值：无
参数：无
*/
void Judge_RightOrWrong()
{
	if(Compare_flag==1)  //表示用户密码输入完成，可以进行比较了
	{
		Compare_flag=0;
		//以下代码为往AT24C02的初始地址为0的连续8个空间内读取8位密码并放在Value1[]数组中，和用户输入的Value2[]密码比较

		At24c02Read(0,Value1);

		//以下代码为往AT24C02的初始地址为0的连续8个空间内读取8位密码并放在Value1[]数组中，和用户输入的Value2[]密码比较
		if(Value1[0]==Value2[0]&&Value1[1]==Value2[1]&&Value1[2]==Value2[2]&&Value1[3]==Value2[3]
			&&Value1[4]==Value2[4]&&Value1[5]==Value2[5]&&Value1[6]==Value2[6]&&Value1[7]==Value2[7]) //如果密码正确
	   	{
			Display(CDIS4,CDIS5);//LCD1602的固定显示部分
			LightOrClose(green,led_on);//输入正确时显示绿灯
			Timer_flag=0;        //表示此时的定时器中断是开门成功了
			BuzzerOpenOrClose(buzzer_on);
			AllowModify_flag=1;  //输对密码之后表示可以修改密码，选择Mode键
			count=0;
			EX0=0;      //关闭外部中断0，无法输数
			TR0=1;		//开定时器0
		}
		else            //至少有一个密码错误
		{
			Display(CDIS3,CDIS2);  //LCD1602的固定显示部分
			count++;               //错误的次数
			if(count==3)           //三次输入错误
			{
				count=0;          //清零，以便下一个三次错误显示
				Timer_flag=1;     //输入的密码次数等于3次了
				LightOrClose(red,led_on);//输入正确时显示绿灯
				BuzzerOpenOrClose(buzzer_on);
				EX0=0;            //关闭外部中断0，无法输数
				TR0=1;		      //开定时器0
			}
			else
			{  
			    Timer_flag=2; //用户输错的密码次数小于3
			    EX0=0;        //关闭外部中断0，无法输数
			    TR0=1;		  //开定时器0
			}			   
		}
	}		
}


/*
功能描述：LCD1602的具体显示内容
返回值：无
参数：	显示在1602第一排的字母CDIS1[];显示在1602第二排的字母CDIS2[]
*/
void Display(uchar CDIS1[],uchar CDIS2[])
{	 
    uchar i;
	lcdwritecom(0x80);
	for(i=0;i<16;i++)
	{
		lcdwritedate(CDIS1[i]);
	}	
	lcdwritecom(0x80+0x40);
	for(i=0;i<16;i++)
	{
		lcdwritedate(CDIS2[i]);	
	}
}

