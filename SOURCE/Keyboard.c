#include"Keyboard.h"

uchar number=0;                            //��¼������û�������������
uchar code CDIS14[]={"CODE NOT ENOUGH!"};  //��ʾ�û����������λ������
uchar code CDIS15[]={"PLEASE CONTINUE!"};
uchar code CDIS16[]={" NO INPUT CODE! "};  //��ʾ�û���û����������Ͱ����������
uchar code CDIS17[]={"ENTER THE CODE! "};
/*
���ܣ�����ֵɨ��
�βΣ���
����ֵ����
*/
void Key_scan()
{
	Key=0xF0;
	if(Key!=0xF0)   //�ж��Ƿ��а�������
	{
		Delayms(200);  //ȥ����ʱ
		if(Key!=0xF0)
			IdentifiCol();  //ȷ����
	}
}
/*
���ܣ�ȷ�ϰ�������
�βΣ���
����ֵ����
*/
void IdentifiCol()
{
	switch(Key)
	{
		case 0xE0:IdentifiRow(0);break;	 //��һ��		
		case 0xD0:IdentifiRow(1);break;	 //�ڶ���
		case 0xB0:IdentifiRow(2);break;	 //������
		case 0x70:IdentifiRow(3);break;	 //������
	}
}

/*
���ܣ�ȷ�ϰ�������
�βΣ�mΪ����
����ֵ����
*/
void IdentifiRow(uchar m)    //ȷ����
{	 
	 Key=0x0F;	  //��Ҫ��һ��
	 switch(Key)
	 {
		case 0x0D:Judge_Value(m);break;    //��һ��
		case 0x0B:Judge_Value(m+4);break;  //�ڶ���
		case 0x07:Judge_Value(m+8);break;  //������
	 }
}

/*
���ܣ���ʾ������ֵ
�βΣ�������ֵn
����ֵ����
*/
void Judge_Value(uchar n)        //��ѡ��ʾ����
{
	uchar a=0,i=0;;
	while((a<50) && (Key!=0x0F)) //��ⰴ�����ּ��
	{
		Delayms(100);
		a++;
	}
	//���´������1s������ʾ��
	BuzzerOpenOrClose(buzzer_on);
	while(i<5)
	{
		i++;
	   	Delayms(200);
	}
	BuzzerOpenOrClose(buzzer_off);

	switch (ConvertToCode(n))
	{
		case 0:if(number==0)		  //�û��ڻ�û����������Ͱ�������
			   {
			   		Display(CDIS16,CDIS17);
					i=0;
					while(i<250)
					{
						i++;
					   	Delayms(250);
					}
					Display(CDIS1,CDIS2);   //�����û���������
			   }
			   else	                  //�û����Ѿ����������������°�����������
			   {	
			   		number--;         //Ϊ������֮ǰ�ġ�*����,����֮ǰ�ġ�*���Ŵ���ʾ��-����
					lcdwritecom(0xc0+0x07+number);
					lcdwritedate('-');	
			   }
			   break;
		case 2:if(number<=7)       //�û��ڻ�û�����빻8λ����Ͱ���ȷ�ϼ�
				{
					Display(CDIS14,CDIS15);
					i=0;
					while(i<250)
					{
						i++;
					   	Delayms(250);
					}
			  		Display(CDIS1,CDIS2);  //�����û���������
					for(i=0;i<number;i++)
					{
						lcdwritecom(0xc0+0x07+i); //���ұ���ס֮ǰ��������룬���û���������
						lcdwritedate('*');
					}
				}
				else			//˵���û���������8λ����֮������ȷ�ϼ�
				{
					Compare_flag=1;		 //����ȥ��ԭʼ����Ƚ���
					number=0;
				}
				break;
		default :					//�û��������������0��9֮�������
				if(number<8)        //��ֹ�û������빻8λ����֮�󻹼����������֣���ʱnumber���ӣ�ֱ���û�����ȷ�ϼ�
				{
					lcdwritecom(0xc0+0x07+number);
					lcdwritedate('*');
					number++;
				}
				break;
	}
}

/*
��������������������û��������Ϣ�������ؽ��
����ֵ��ת��֮��ĺ���ֵ��ȷ�ϵ���Ϣ���Լ������Ѿ�����İ���ֵ
������	���������û���������루����1 2 3 4��
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
		case 10: return 0;break;    //��ʾ�û�������ǰ���Ҫ�����һ���Ѿ����������
		case 11: return 2;break;	//��ʾ�û�������ȷ�ϼ�
	}
}


