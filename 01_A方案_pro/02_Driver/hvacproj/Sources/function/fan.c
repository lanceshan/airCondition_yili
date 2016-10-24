/*
 * fan.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "fan.h"

void Set_OutsideFan_Speed(unsigned char fanLv)
{
	unsigned int ratio=0;
	unsigned char frq=0;
	switch (fanLv) 
	{
		case 0:
			frq = OUTSIDE_FAN_OFF;
			break;
		case 1:
			frq = OUTSIDE_FAN_LOW;
			break;
		case 2:
			frq = OUTSIDE_FAN_HIGH;
			break;
		default:
			break;
	}
	ratio = (unsigned int)(65535*(100-frq)/100);
	OutsideFan_PWM_SetRatio16(ratio);
}

void Set_InnerFan_Speed(unsigned char windLv)
{
	unsigned int ratio=0;
	unsigned char frq=0;
	switch (windLv) 
	{
		case 0:
			frq = Blower_Lv0;		//PWM<10%,close blower
			break;
		case 1:
			frq = Blower_Lv1;
			break;
		case 2:
			frq = Blower_Lv2;
			break;
		case 3:
			frq = Blower_Lv3;
			break;
		case 4:
			frq = Blower_Lv4;
			break;
		case 5:
			frq = Blower_Lv5;
			break;
		case 6:
			frq = Blower_Lv6;
			break;
		default:
			break;
	}
	ratio = (unsigned int)(65535*(100-frq)/100);
	InnerFan_PWM_SetRatio16(ratio);
}

void Open_StoreFan(void)
{
	StoreFan_Ctrl_SetVal();
}
void Close_StoreFan(void)
{
	StoreFan_Ctrl_ClrVal();
}

