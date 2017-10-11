#include"Interrupt.h"



uchar code table[]= {"   20  -  -     "};     //定义1602上必有的DS1302的显示
uchar code table1[]={"      :  :      "}; 


/*
功能描述：外部中断0，读取红外发出的数据
返回值：无
参数：无
*/
void Read_infrared_data() interrupt 0	 //外部中断零
{
	uchar num,k,j,i=0;   //num用来区分1和0高电平长度的区别
	uint num1;       //num1用来确认是否正确
	EX0=0;           //关掉外部中断，因为后面还有很多次的高低电平的跳变
	num=0;				 
	Delayms(50);
	/*以下代码为红外检测的固定代码*/
	if(IRIN==0)		//确认是否真的接收到正确的信号
	{	 		
		num1=1000;			     	//1000*10us=10ms,超过说明接收到错误的信号
		while((IRIN==0)&&(num1>0))	//等待前面9ms的低电平过去  		
		{			
			Delayms(1);
			num1--;
		} 
		if(IRIN==1)			//如果正确等到9ms低电平
		{
			num1=500;
			while((IRIN==1)&&(num1>0))		 //等待4.5ms的起始高电平过去
			{
				Delayms(1);
				num1--;
			}	
	
			for(k=0;k<4;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					num1=60;		
					while((IRIN==0)&&(num1>0))//等待信号前面的560us低电平过去
					{
						Delayms(1);
						num1--;
					}
					num1=500;
					while((IRIN==1)&&(num1>0))	 //计算高电平的时间长度。
					{
						Delayms(1);//0.14ms
						num++;
						num1--;
						if(num>25)// 本来如果是1的话最多是0.56毫秒低电平加上1.68毫秒的高电平总共2.25的时间，
						{		  //如果大于2.25的话那就错了，开外部中断，跳出中断函数
							EX0=1;
							return;
						}
					}
					Value[k]>>=1;	    //k表示第几组数据
					if(num>=10)			//如果高电平出现大于565us，那么是1
					{
						Value[k]|=0x80;
					}
					num=0;		//用完时间要重新赋值							
				}
			}
		}
		if(Value[0]!=~Value[1])//地址码与地址反码的验证
		{
			EX0=1;
			return;
		}
		if(Value[2]!=~Value[3])//命令码与命令反码的验证
		{
			EX0=1;
			return;
		}
        /*以上代码为红外检测的固定代码*/

		//以下代码产生1s按键提示音
		i=0;
		BuzzerOpenOrClose(buzzer_on);
		while(i<5)
		{
			i++;
		   	Delayms(200);
		}
		BuzzerOpenOrClose(buzzer_off);
		/*以下代码为检测是否要一键开门*/ 
		if(Value[2]==0x45)
		{
			Display(CDIS4,CDIS5);  //LCD1602的固定显示部分
			LightOrClose(green,led_on);//输入正确时显示绿灯
			BuzzerOpenOrClose(buzzer_on);
			Timer_flag=0;          //表示此时的定时器中断是开门成功了
			count=0;	 
			EX0=0;      //关闭外部中断0，无法输数
			TR0=1;		//开定时器0
			return;
		}
		/*以下代码为检测是否要显示时间*/
		else if(Value[2]==0x47)
		{
			Timer_flag=3;          //表示此时的定时器中断是开门成功了
			Display(table,table1);  //LCD1602的固定显示部分	
			Transformation(); //时间显示函数 
			EX0=0;      //关闭外部中断0，无法输数
			TR0=1;		//开定时器0
			return;
		}

		/*以下代码为检测是否设定新的密码*/
		else if(AllowModify_flag==1&&Value[2]==0x46) //接收到允许修改密码的命令Mode，要修改密码
		{
			num2=0;
			Display(CDIS6,CDIS7); //LCD1602的固定显示部分(ENTER THE NEW CODE!)
			EX0=1;               //开中断，允许去依次接收新密码
			AllowModify_flag=0;  //设置新密码的标志位清零
			Modify_flag=1;       //为了依次放新密码到Value1[4]的数组中
			return ;
		}
		/*以上代码为检测是否设定新的密码*/

		/*以下代码为检测输入的密码是否有效，即是否在0到9之间*/
		else
			if(!Judge_password(Value))
			{
				EX0=1;
				return;	  //密码无效就继续接收直到有效为止
			}

		Input_Password(); //将用户输入的密码存入到一个数组中
	} 
	EX0=1;  //开外部中断
}

/*
功能描述：定时器0中断服务函数
返回值：无
参数：无
*/
void Timer0() interrupt 1
{
	TH0=0X0FA;  //初值64000.计数1536个数
	TL0=0X00;
	switch(Timer_flag)
	{
		case 0:	                
		    T_count++;
			if(T_count==500) //蜂鸣器1s提示音表示密码正确或者设置密码完成不论成功与否, 无源蜂鸣器内部没有振荡源，所以要人为的输入2K~5K的方波
				BuzzerOpenOrClose(buzzer_off); //关蜂鸣器 	
			if(T_count==5000)//说明5s时间到了，门要重新关闭了
			{
				T_count=0;
				LightOrClose(green,led_off);     //关绿灯,门重新关闭
				Display(CDIS1,CDIS2);            //LCD1602的固定显示部分,门关闭之后重新显示输入密码的界面
				TR0=0;	//关定时器0				  
				EX0=1;  //开外部中断0
			}
			break;
		case 1:
		     T_count++;	
		 	 if(T_count==5000)////输错密码次数等于3，报警4s,蜂鸣器响4s
			 {
			 	T_count=0;
				BuzzerOpenOrClose(buzzer_off); //关蜂鸣器
				LightOrClose(red,led_off);     //关红灯
				TR0=0;	//关定时器0					  
				EX0=1;  //开外部中断0
			 }
			 break;
	    case 2:
			 T_count++;
			 if(T_count%30==0)
			    led1=!led1;    //红灯闪烁	
		 	 if(T_count==800)  //输错密码次数小于3报警2s
			 {
			 	T_count=0;
				LightOrClose(red,led_off);     //关红灯
				TR0=0;	//关定时器0					  
				EX0=1;  //开外部中断0
			 }
			 break;
		default :
		     T_count++;	
		 	 if(T_count==2000)  //输错密码次数小于3报警2s
		     {
			    T_count=0;
			   	Display(CDIS1,CDIS2);
				TR0=0;	//关定时器0					  
				EX0=1;  //开外部中断0
			 }
			 break;					     
	}
}


/*
功能描述：4位密码的存入一个数组（无论新旧密码）
返回值：无
参数：	无
*/
void Input_Password()
{
	/*以下代码为用户输入密码*/
	if(Modify_flag==0) //用户输入密码（无论新密码还是旧密码）
	{
		if(Value[2]!=0x07)	 //防止用户把设置新密码的确认键0x70当成有效密码
		{
			Shown_Asterisks(num2);  //把用用户输入的信息在LCD上面显示*号保护用户密码安全
			Value2[num2]=Value[2]; //把用户重新输入的密码放到Value2[]中
			num2++;
			if(num2==8)
			{
				num2=0;
				Compare_flag=1;   //用户输完密码之后，可以和设定的密码比较了
			}
		}
	}
	/*以上代码为用户输入密码*/

	/*此下代码为用户更换新密码*/
	else
	{
		Shown_Asterisks(num2); //把用用户输入的信息在LCD上面显示*号保护用户密码安全
		Value3[num2]=Value[2];  //Value是命令码，即数据码，设定新密码并先放到Value2临时数组中，如果和再次确认密码吻合
		                        //就把Value2[]中的密码拷贝到Value1[]中作为新密码


		if(num2<=7&&Value[2]==0x07)		//表示用户设置密码时不够8位就按下了确认键
		{
			num2=0;

		   //以下代码为表示用户输入的新密码位数不够而终止，并且恢复初始状态
			Display(CDIS12,CDIS13); //LCD1602的固定显示部分,提示用户设置密码失败

			delay_count=0;
			while(delay_count<255)	//短延时让用户看到设置密码的提示结果
			{				
				Delayms(255);
				delay_count++;
			}

			Display(CDIS1,CDIS2);  //LCD1602的固定显示部分,用户设置密码失败后显示输入密码的界面
			Modify_flag=0;         //表示系统恢复正常，回到初始界面
			//以上代码为表示用户输入的新密码位数不够而终止，并且恢复初始状态

			return ;
		}
		num2++;		
		switch(First_flag)	   //判断是第一次设置密码还是第二次设置密码
		{
			case 0:				//第一次设置密码
				Value2[num2-1]=Value3[num2-1]; //将第一次设置的密码存储在Value3[]中，好与第二次输入的存储在Value3中的密码比较

				if(num2==9)	 //用户输够新密码之后并按下了确认键
				{
					if(Value[2]==0x07)
					{
						num2=0;
						First_flag=1;         //第一次设置新密码成功，在Value2[]中存储
						Display(CDIS8,CDIS9); //LCD1602的固定显示部分,提示用户再一次输入新的密码
					}
					else 
						num2--;		  //防止用户按够了8位密码后还继续按数字键
				}
				break;
		  default :			                //第二次设置密码
				if(num2==9)	 //用户输够新密码之后并按下了确认键
				{
					if(Value[2]==0x07)
					{
						num2=0;
						Second_flag=1; //第二次设置密码成功，在Value3[]中存储
						Modify_flag=0;
					}
					else
						num2--;	 //防止用户按够了8位密码后还继续按数字键
				}
				break;
		}
	}
    /*此上代码为用户更换新密码*/	
}


/*
功能描述：判断用户输入的密码是不是0~9之内 
返回值：是密码有效就返回1，否则返回0
参数：	将红外接收到的有效数据作为形参
*/
uchar Judge_password(uchar value[])
{
	switch(value[2])
	{
		case 0x16: return 1;break;
		case 0x0c: return 1;break;
		case 0x18: return 1;break;
		case 0x5e: return 1;break;
		case 0x08: return 1;break;
		case 0x1c: return 1;break;
		case 0x5a: return 1;break;
		case 0x42: return 1;break;
		case 0x52: return 1;break;
		case 0x4a: return 1;break;
		case 0x07: return 1;break;
		default :  return 0;break;
	}
}


/*
功能描述：把用用户输入的信息在LCD上面显示*号保护用户密码安全
返回值：无
参数：	现在用户输入的密码的个数num2
*/
void  Shown_Asterisks(uchar num2)
{
	switch(num2)   //记录输入的个数，显示*号，保护用户的信息部丢失
	{
		case 0: lcdwritecom(0xc0+0x07);
				lcdwritedate('*');
				break;
		case 1: lcdwritecom(0xc0+0x08);
				lcdwritedate('*');
				break;
		case 2: lcdwritecom(0xc0+0x09);
				lcdwritedate('*');
				break;
		case 3: lcdwritecom(0xc0+0x0A);
				lcdwritedate('*');
				break;
		case 4: lcdwritecom(0xc0+0x0B);
				lcdwritedate('*');
				break;
		case 5: lcdwritecom(0xc0+0x0C);
				lcdwritedate('*');
				break;
		case 6: lcdwritecom(0xc0+0x0D);
				lcdwritedate('*');
				break;
		case 7: lcdwritecom(0xc0+0x0E);
				lcdwritedate('*');
				break;
	}
}

