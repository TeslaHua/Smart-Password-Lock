#ifndef __LedAndBuzzer_H_
#define __LedAndBuzzer_H_
#include<Base.h>


#define led_off  1    //表示关灯
#define led_on 0	 //表示开灯
#define green 1		 //表示绿灯
#define red 0		 //表示红灯
#define buzzer_on 0  //表示蜂鸣器响
#define buzzer_off 1  //表示蜂鸣器停止响

void LightOrClose(uchar led, uchar state);   //把一颗灯点亮
void BuzzerOpenOrClose(uchar state);

					  
#endif