#include"Keyboard.h"

uchar number=0;                            //记录门外的用户输入的密码个数
uchar code CDIS14[]={"CODE NOT ENOUGH!"};  //提示用户输入的密码位数不够
uchar code CDIS15[]={"PLEASE CONTINUE!"};
uchar code CDIS16[]={" NO INPUT CODE! "};  //提示用户还没有输入密码就按下了清除键
uchar code CDIS17[]={"ENTER THE CODE! "};
/*
功能：按键值扫描
形参：无
返回值：无
*/
void Key_scan()
{
	Key=0xF0;
	if(Key!=0xF0)   //判断是否有按键按下
	{
		Delayms(200);  //去抖延时
		if(Key!=0xF0)
			IdentifiCol();  //确认列
	}
}
/*
功能：确认按键的列
形参：无
返回值：无
*/
void IdentifiCol()
{
	switch(Key)
	{
		case 0xE0:IdentifiRow(0);break;	 //第一列		
		case 0xD0:IdentifiRow(1);break;	 //第二列
		case 0xB0:IdentifiRow(2);break;	 //第三列
		case 0x70:IdentifiRow(3);break;	 //第四列
	}
}

/*
功能：确认按键的行
形参：m为列数
返回值：无
*/
void IdentifiRow(uchar m)    //确认行
{	 
	 Key=0x0F;	  //重要的一步
	 switch(Key)
	 {
		case 0x0D:Judge_Value(m);break;    //第一行
		case 0x0B:Judge_Value(m+4);break;  //第二行
		case 0x07:Judge_Value(m+8);break;  //第三行
	 }
}

/*
功能：显示按键的值
形参：按键的值n
返回值：无
*/
void Judge_Value(uchar n)        //段选显示函数
{
	uchar a=0,i=0;;
	while((a<50) && (Key!=0x0F)) //检测按键松手检测
	{
		Delayms(100);
		a++;
	}
	//以下代码产生1s按键提示音
	BuzzerOpenOrClose(buzzer_on);
	while(i<5)
	{
		i++;
	   	Delayms(200);
	}
	BuzzerOpenOrClose(buzzer_off);

	switch (ConvertToCode(n))
	{
		case 0:if(number==0)		  //用户在还没有输入密码就按消除键
			   {
			   		Display(CDIS16,CDIS17);
					i=0;
					while(i<250)
					{
						i++;
					   	Delayms(250);
					}
					Display(CDIS1,CDIS2);   //提醒用户输入密码
			   }
			   else	                  //用户在已经有密码输入的情况下按下了消除键
			   {	
			   		number--;         //为了消除之前的“*”键,并在之前的“*”号处显示“-”号
					lcdwritecom(0xc0+0x07+number);
					lcdwritedate('-');	
			   }
			   break;
		case 2:if(number<=7)       //用户在还没有输入够8位密码就按下确认键
				{
					Display(CDIS14,CDIS15);
					i=0;
					while(i<250)
					{
						i++;
					   	Delayms(250);
					}
			  		Display(CDIS1,CDIS2);  //提醒用户输入密码
					for(i=0;i<number;i++)
					{
						lcdwritecom(0xc0+0x07+i); //并且保留住之前输入的密码，让用户接着输入
						lcdwritedate('*');
					}
				}
				else			//说明用户在输入完8位密码之后按下了确认键
				{
					Compare_flag=1;		 //可以去和原始密码比较了
					number=0;
				}
				break;
		default :					//用户输入的是正常的0到9之间的数字
				if(number<8)        //防止用户在输入够8位密码之后还继续输入数字，此时number不加，直到用户输入确认键
				{
					lcdwritecom(0xc0+0x07+number);
					lcdwritedate('*');
					number++;
				}
				break;
	}
}

/*
功能描述：处理门外的用户输入的信息，并返回结果
返回值：转换之后的红外值或确认的信息，以及消除已经输出的按键值
参数：	现在门外用户输入的密码（例如1 2 3 4）
*/
uchar ConvertToCode(uchar n)
{
	switch (n)
	{
		case 0: Value2[number]=0x16;return 1;break;
		case 1:	Value2[number]=0x0c;return 1;break;
		case 2:	Value2[number]=0x18;return 1;break;
		case 3:	Value2[number]=0x5e;return 1;break;
		case 4:	Value2[number]=0x08;return 1;break;
		case 5:	Value2[number]=0x1c;return 1;break;
		case 6:	Value2[number]=0x5a;return 1;break;
		case 7:	Value2[number]=0x42;return 1;break;
		case 8:	Value2[number]=0x52;return 1;break;
		case 9:	Value2[number]=0x4a;return 1;break;
		case 10: return 0;break;    //表示用户输入的是按键要求清除一个已经输入的密码
		case 11: return 2;break;	//表示用户按下了确认键
	}
}


