#include<LedAndBuzzer.h>

/*
功能：将单个led灯点亮或者熄灭的功能
形参：led 红灯还是绿灯，state 亮还是灭
返回值：无
*/
void LightOrClose(uchar led, uchar state)      //把一颗灯点亮
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
功能: 控制蜂鸣器是否发声
形参：state 开还是关
返回值：无
*/
void BuzzerOpenOrClose(uchar state)
{
	Beep=state>0?1:0;
}