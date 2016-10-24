/*
 * compressor.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "compressor.h"

void Close_Cmp(unsigned char *p_cmpLv, unsigned int *p_tcnt)
{
	if(*p_cmpLv == 0)return;
	
	if(*p_tcnt == 0) 		//压缩机频率变化时间间隔
	{
		switch (*p_cmpLv)	//逐渐降低压缩机频率，直到关闭
		{
			case 8:
				*p_cmpLv = 7;
				break;
			case 7:
				*p_cmpLv = 6;
				break;
			case 6:
				*p_cmpLv = 5;
				break;
			case 5:
				*p_cmpLv = 4;
				break;
			case 4:
				*p_cmpLv = 3;
				break;
			case 3:
				*p_cmpLv = 2;
				break;
			case 2:
				*p_cmpLv = 1;
				break;
			case 1:
				*p_cmpLv = 0;
				break;
			default:
				break;	
		}
		Set_CmpLv(*p_cmpLv);
		*p_tcnt = CMP_SWITCH_TIME;
	}
}

void Set_CmpLv(unsigned char cmpLv)
{
	if(cmpLv > 6)cmpLv = 6;
	switch (cmpLv)
	{
		case 8:
			MI1_ON;
			MI3_ON;
			MI4_ON;
			MI5_ON;
			break;
		case 7:
			MI1_ON;
			MI3_OFF;
			MI4_ON;
			MI5_ON;
			break;
		case 6:
			MI1_ON;
			MI3_ON;
			MI4_OFF;
			MI5_ON;
			break;
		case 5:
			MI1_ON;
			MI3_OFF;
			MI4_OFF;
			MI5_ON;
			break;
		case 4:
			MI1_ON;
			MI3_ON;
			MI4_ON;
			MI5_OFF;
			break;
		case 3:
			MI1_ON;
			MI3_OFF;
			MI4_ON;
			MI5_OFF;
			break;
		case 2:
			MI1_ON;
			MI3_ON;
			MI4_OFF;
			MI5_OFF;
			break;
		case 1:
			MI1_ON;
			MI3_OFF;
			MI4_OFF;
			MI5_OFF;
			break;
		case 0:	//关闭压缩机
			MI1_OFF;
			MI3_OFF;
			MI4_OFF;
			MI5_OFF;
			break;
		default:
			break;	
	}
}
void Set_Cmp(unsigned char cmpLv)
{
	Set_CmpLv(cmpLv);
}
