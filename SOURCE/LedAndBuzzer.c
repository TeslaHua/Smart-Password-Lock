#include<LedAndBuzzer.h>

/*
���ܣ�������led�Ƶ�������Ϩ��Ĺ���
�βΣ�led ��ƻ����̵ƣ�state ��������
����ֵ����
*/
void LightOrClose(uchar led, uchar state)      //��һ�ŵƵ���
{
	switch (led)
	{
		case 1:
			  led0=state>0?1:0;
			  break;
		case 0:
			  led1=state>0?1:0;
			  break;
	}
}

/*
����: ���Ʒ������Ƿ���
�βΣ�state �����ǹ�
����ֵ����
*/
void BuzzerOpenOrClose(uchar state)
{
	Beep=state>0?1:0;
}