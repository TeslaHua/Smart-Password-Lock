#include"Interrupt.h"



uchar code table[]= {"   20  -  -     "};     //����1602�ϱ��е�DS1302����ʾ
uchar code table1[]={"      :  :      "}; 


/*
�����������ⲿ�ж�0����ȡ���ⷢ��������
����ֵ����
��������
*/
void Read_infrared_data() interrupt 0	 //�ⲿ�ж���
{
	uchar num,k,j,i=0;   //num��������1��0�ߵ�ƽ���ȵ�����
	uint num1;       //num1����ȷ���Ƿ���ȷ
	EX0=0;           //�ص��ⲿ�жϣ���Ϊ���滹�кܶ�εĸߵ͵�ƽ������
	num=0;				 
	Delayms(50);
	/*���´���Ϊ������Ĺ̶�����*/
	if(IRIN==0)		//ȷ���Ƿ���Ľ��յ���ȷ���ź�
	{	 		
		num1=1000;			     	//1000*10us=10ms,����˵�����յ�������ź�
		while((IRIN==0)&&(num1>0))	//�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
		{			
			Delayms(1);
			num1--;
		} 
		if(IRIN==1)			//�����ȷ�ȵ�9ms�͵�ƽ
		{
			num1=500;
			while((IRIN==1)&&(num1>0))		 //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
			{
				Delayms(1);
				num1--;
			}	
	
			for(k=0;k<4;k++)		//����4������
			{				
				for(j=0;j<8;j++)	//����һ������
				{

					num1=60;		
					while((IRIN==0)&&(num1>0))//�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
					{
						Delayms(1);
						num1--;
					}
					num1=500;
					while((IRIN==1)&&(num1>0))	 //����ߵ�ƽ��ʱ�䳤�ȡ�
					{
						Delayms(1);//0.14ms
						num++;
						num1--;
						if(num>25)// ���������1�Ļ������0.56����͵�ƽ����1.68����ĸߵ�ƽ�ܹ�2.25��ʱ�䣬
						{		  //�������2.25�Ļ��Ǿʹ��ˣ����ⲿ�жϣ������жϺ���
							EX0=1;
							return;
						}
					}
					Value[k]>>=1;	    //k��ʾ�ڼ�������
					if(num>=10)			//����ߵ�ƽ���ִ���565us����ô��1
					{
						Value[k]|=0x80;
					}
					num=0;		//����ʱ��Ҫ���¸�ֵ							
				}
			}
		}
		if(Value[0]!=~Value[1])//��ַ�����ַ�������֤
		{
			EX0=1;
			return;
		}
		if(Value[2]!=~Value[3])//����������������֤
		{
			EX0=1;
			return;
		}
        /*���ϴ���Ϊ������Ĺ̶�����*/

		//���´������1s������ʾ��
		i=0;
		BuzzerOpenOrClose(buzzer_on);
		while(i<5)
		{
			i++;
		   	Delayms(200);
		}
		BuzzerOpenOrClose(buzzer_off);
		/*���´���Ϊ����Ƿ�Ҫһ������*/ 
		if(Value[2]==0x45)
		{
			Display(CDIS4,CDIS5);  //LCD1602�Ĺ̶���ʾ����
			LightOrClose(green,led_on);//������ȷʱ��ʾ�̵�
			BuzzerOpenOrClose(buzzer_on);
			Timer_flag=0;          //��ʾ��ʱ�Ķ�ʱ���ж��ǿ��ųɹ���
			count=0;	 
			EX0=0;      //�ر��ⲿ�ж�0���޷�����
			TR0=1;		//����ʱ��0
			return;
		}
		/*���´���Ϊ����Ƿ�Ҫ��ʾʱ��*/
		else if(Value[2]==0x47)
		{
			Timer_flag=3;          //��ʾ��ʱ�Ķ�ʱ���ж��ǿ��ųɹ���
			Display(table,table1);  //LCD1602�Ĺ̶���ʾ����	
			Transformation(); //ʱ����ʾ���� 
			EX0=0;      //�ر��ⲿ�ж�0���޷�����
			TR0=1;		//����ʱ��0
			return;
		}

		/*���´���Ϊ����Ƿ��趨�µ�����*/
		else if(AllowModify_flag==1&&Value[2]==0x46) //���յ������޸����������Mode��Ҫ�޸�����
		{
			num2=0;
			Display(CDIS6,CDIS7); //LCD1602�Ĺ̶���ʾ����(ENTER THE NEW CODE!)
			EX0=1;               //���жϣ�����ȥ���ν���������
			AllowModify_flag=0;  //����������ı�־λ����
			Modify_flag=1;       //Ϊ�����η������뵽Value1[4]��������
			return ;
		}
		/*���ϴ���Ϊ����Ƿ��趨�µ�����*/

		/*���´���Ϊ�������������Ƿ���Ч�����Ƿ���0��9֮��*/
		else
			if(!Judge_password(Value))
			{
				EX0=1;
				return;	  //������Ч�ͼ�������ֱ����ЧΪֹ
			}

		Input_Password(); //���û������������뵽һ��������
	} 
	EX0=1;  //���ⲿ�ж�
}

/*
������������ʱ��0�жϷ�����
����ֵ����
��������
*/
void Timer0() interrupt 1
{
	TH0=0X0FA;  //��ֵ64000.����1536����
	TL0=0X00;
	switch(Timer_flag)
	{
		case 0:	                
		    T_count++;
			if(T_count==500) //������1s��ʾ����ʾ������ȷ��������������ɲ��۳ɹ����, ��Դ�������ڲ�û����Դ������Ҫ��Ϊ������2K~5K�ķ���
				BuzzerOpenOrClose(buzzer_off); //�ط����� 	
			if(T_count==5000)//˵��5sʱ�䵽�ˣ���Ҫ���¹ر���
			{
				T_count=0;
				LightOrClose(green,led_off);     //���̵�,�����¹ر�
				Display(CDIS1,CDIS2);            //LCD1602�Ĺ̶���ʾ����,�Źر�֮��������ʾ��������Ľ���
				TR0=0;	//�ض�ʱ��0				  
				EX0=1;  //���ⲿ�ж�0
			}
			break;
		case 1:
		     T_count++;	
		 	 if(T_count==5000)////��������������3������4s,��������4s
			 {
			 	T_count=0;
				BuzzerOpenOrClose(buzzer_off); //�ط�����
				LightOrClose(red,led_off);     //�غ��
				TR0=0;	//�ض�ʱ��0					  
				EX0=1;  //���ⲿ�ж�0
			 }
			 break;
	    case 2:
			 T_count++;
			 if(T_count%30==0)
			    led1=!led1;    //�����˸	
		 	 if(T_count==800)  //����������С��3����2s
			 {
			 	T_count=0;
				LightOrClose(red,led_off);     //�غ��
				TR0=0;	//�ض�ʱ��0					  
				EX0=1;  //���ⲿ�ж�0
			 }
			 break;
		default :
		     T_count++;	
		 	 if(T_count==2000)  //����������С��3����2s
		     {
			    T_count=0;
			   	Display(CDIS1,CDIS2);
				TR0=0;	//�ض�ʱ��0					  
				EX0=1;  //���ⲿ�ж�0
			 }
			 break;					     
	}
}


/*
����������4λ����Ĵ���һ�����飨�����¾����룩
����ֵ����
������	��
*/
void Input_Password()
{
	/*���´���Ϊ�û���������*/
	if(Modify_flag==0) //�û��������루���������뻹�Ǿ����룩
	{
		if(Value[2]!=0x07)	 //��ֹ�û��������������ȷ�ϼ�0x70������Ч����
		{
			Shown_Asterisks(num2);  //�����û��������Ϣ��LCD������ʾ*�ű����û����밲ȫ
			Value2[num2]=Value[2]; //���û��������������ŵ�Value2[]��
			num2++;
			if(num2==8)
			{
				num2=0;
				Compare_flag=1;   //�û���������֮�󣬿��Ժ��趨������Ƚ���
			}
		}
	}
	/*���ϴ���Ϊ�û���������*/

	/*���´���Ϊ�û�����������*/
	else
	{
		Shown_Asterisks(num2); //�����û��������Ϣ��LCD������ʾ*�ű����û����밲ȫ
		Value3[num2]=Value[2];  //Value�������룬�������룬�趨�����벢�ȷŵ�Value2��ʱ�����У�������ٴ�ȷ�������Ǻ�
		                        //�Ͱ�Value2[]�е����뿽����Value1[]����Ϊ������


		if(num2<=7&&Value[2]==0x07)		//��ʾ�û���������ʱ����8λ�Ͱ�����ȷ�ϼ�
		{
			num2=0;

		   //���´���Ϊ��ʾ�û������������λ����������ֹ�����һָ���ʼ״̬
			Display(CDIS12,CDIS13); //LCD1602�Ĺ̶���ʾ����,��ʾ�û���������ʧ��

			delay_count=0;
			while(delay_count<255)	//����ʱ���û����������������ʾ���
			{				
				Delayms(255);
				delay_count++;
			}

			Display(CDIS1,CDIS2);  //LCD1602�Ĺ̶���ʾ����,�û���������ʧ�ܺ���ʾ��������Ľ���
			Modify_flag=0;         //��ʾϵͳ�ָ��������ص���ʼ����
			//���ϴ���Ϊ��ʾ�û������������λ����������ֹ�����һָ���ʼ״̬

			return ;
		}
		num2++;		
		switch(First_flag)	   //�ж��ǵ�һ���������뻹�ǵڶ�����������
		{
			case 0:				//��һ����������
				Value2[num2-1]=Value3[num2-1]; //����һ�����õ�����洢��Value3[]�У�����ڶ�������Ĵ洢��Value3�е�����Ƚ�

				if(num2==9)	 //�û��乻������֮�󲢰�����ȷ�ϼ�
				{
					if(Value[2]==0x07)
					{
						num2=0;
						First_flag=1;         //��һ������������ɹ�����Value2[]�д洢
						Display(CDIS8,CDIS9); //LCD1602�Ĺ̶���ʾ����,��ʾ�û���һ�������µ�����
					}
					else 
						num2--;		  //��ֹ�û�������8λ����󻹼��������ּ�
				}
				break;
		  default :			                //�ڶ�����������
				if(num2==9)	 //�û��乻������֮�󲢰�����ȷ�ϼ�
				{
					if(Value[2]==0x07)
					{
						num2=0;
						Second_flag=1; //�ڶ�����������ɹ�����Value3[]�д洢
						Modify_flag=0;
					}
					else
						num2--;	 //��ֹ�û�������8λ����󻹼��������ּ�
				}
				break;
		}
	}
    /*���ϴ���Ϊ�û�����������*/	
}


/*
�����������ж��û�����������ǲ���0~9֮�� 
����ֵ����������Ч�ͷ���1�����򷵻�0
������	��������յ�����Ч������Ϊ�β�
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
���������������û��������Ϣ��LCD������ʾ*�ű����û����밲ȫ
����ֵ����
������	�����û����������ĸ���num2
*/
void  Shown_Asterisks(uchar num2)
{
	switch(num2)   //��¼����ĸ�������ʾ*�ţ������û�����Ϣ����ʧ
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

