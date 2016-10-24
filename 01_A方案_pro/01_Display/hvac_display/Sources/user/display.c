/*
 * display.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "display.h"
#include "globalvars.h"

void Set_LCD_Num1(unsigned char num, unsigned char buf[][13])		//中间3个7段数码管中第一个数码管
{
	switch(num)
	{
		case 0:// 1a  			1b				1c				1d				1e				1f				1g				2dot
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 1;	buf[1][10] = 1;	buf[1][9] = 0;	buf[3][7] = 1;
			break;
		case 1:
			buf[0][10] = 0;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 0;	buf[2][10] = 0;	buf[1][10] = 0;	buf[1][9] = 0;	buf[3][7] = 1;
			break;
		case 2:
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 0;	buf[3][10] = 1;	buf[2][10] = 1;	buf[1][10] = 0;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 3:
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 0;	buf[1][10] = 0;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 4:
			buf[0][10] = 0;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 0;	buf[2][10] = 0;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 5:
			buf[0][10] = 1;	buf[0][9] = 0;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 0;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 6:
			buf[0][10] = 1;	buf[0][9] = 0;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 1;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 7:
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 0;	buf[2][10] = 0;	buf[1][10] = 0;	buf[1][9] = 0;	buf[3][7] = 1;
			break;
		case 8:
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 1;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 9:
			buf[0][10] = 1;	buf[0][9] = 1;	buf[2][9] = 1;	buf[3][10] = 1;	buf[2][10] = 0;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 1;
			break;
		case 10://"-"
			buf[0][10] = 0;	buf[0][9] = 0;	buf[2][9] = 0;	buf[3][10] = 0;	buf[2][10] = 0;	buf[1][10] = 0;	buf[1][9] = 1;	buf[3][7] = 0;
			break;
		case 11://第一位数字灭，第二位数字小数点亮
			buf[0][10] = 0;	buf[0][9] = 0;	buf[2][9] = 0;	buf[3][10] = 0;	buf[2][10] = 0;	buf[1][10] = 0;	buf[1][9] = 0;	buf[3][7] = 1;
			break;
		case ERR_E://“ERR的E”
			buf[0][10] = 1;	buf[0][9] = 0;	buf[2][9] = 0;	buf[3][10] = 1;	buf[2][10] = 1;	buf[1][10] = 1;	buf[1][9] = 1;	buf[3][7] = 0;
			break;
		default:
			break;	
	}
}

void Set_LCD_Num2(unsigned char num, unsigned char buf[4][13])		//中间3个7段数码管中第二个数码管
{
	switch(num)
	{
		case 0:// 2a  			2b				2c				2d				2e				2f				2g				
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 1;	buf[1][8] = 1;	buf[1][7] = 0;
			break;
		case 1:
			buf[0][8] = 0;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 0;	buf[2][8] = 0;	buf[1][8] = 0;	buf[1][7] = 0;
			break;
		case 2:
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 0;	buf[3][8] = 1;	buf[2][8] = 1;	buf[1][8] = 0;	buf[1][7] = 1;
			break;
		case 3:
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 0;	buf[1][8] = 0;	buf[1][7] = 1;
			break;
		case 4:
			buf[0][8] = 0;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 0;	buf[2][8] = 0;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		case 5:
			buf[0][8] = 1;	buf[0][7] = 0;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 0;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		case 6:
			buf[0][8] = 1;	buf[0][7] = 0;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 1;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		case 7:
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 0;	buf[2][8] = 0;	buf[1][8] = 0;	buf[1][7] = 0;
			break;
		case 8:
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 1;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		case 9:
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 1;	buf[2][8] = 0;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		case 10://“熄灭”
			buf[0][8] = 0;	buf[0][7] = 0;	buf[2][7] = 0;	buf[3][8] = 0;	buf[2][8] = 0;	buf[1][8] = 0;	buf[1][7] = 0;
			break;
		case ERR_R://“ERR的R”
			buf[0][8] = 1;	buf[0][7] = 1;	buf[2][7] = 1;	buf[3][8] = 0;	buf[2][8] = 1;	buf[1][8] = 1;	buf[1][7] = 1;
			break;
		default:
			break;	
	}
}

void Set_LCD_Num3(unsigned char num, unsigned char buf[4][13])			//中间3个7段数码管中第三个数码管
{
	switch(num)
	{
		case 0:// 3a  			3b				3c				3d				3e				3f				3g				℃
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 1;	buf[1][6] = 1;	buf[1][5] = 0;	buf[3][5] = 1;
			break;
		case 1:
			buf[0][6] = 0;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 0;	buf[2][6] = 0;	buf[1][6] = 0;	buf[1][5] = 0;	buf[3][5] = 1;
			break;
		case 2:
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 0;	buf[3][6] = 1;	buf[2][6] = 1;	buf[1][6] = 0;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 3:
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 0;	buf[1][6] = 0;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 4:
			buf[0][6] = 0;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 0;	buf[2][6] = 0;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 5:
			buf[0][6] = 1;	buf[0][5] = 0;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 0;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 6:
			buf[0][6] = 1;	buf[0][5] = 0;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 1;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 7:
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 0;	buf[2][6] = 0;	buf[1][6] = 0;	buf[1][5] = 0;	buf[3][5] = 1;
			break;
		case 8:
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 1;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case 9:
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 1;	buf[2][6] = 0;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 1;
			break;
		case ERR_R://“ERR的R”
			buf[0][6] = 1;	buf[0][5] = 1;	buf[2][5] = 1;	buf[3][6] = 0;	buf[2][6] = 1;	buf[1][6] = 1;	buf[1][5] = 1;	buf[3][5] = 0;
		default:
			break;
	}
}

void Set_LCD_Num4(unsigned char num, unsigned char buf[4][13])		//底部2个7段数码管中第一个数码管
{
	switch(num)
	{
		case 0:// 4a  			4b				4c				4d				4e				4f				4g				
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 0;
			break;
		case 1:
			buf[3][1] = 0;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 0;	buf[1][1] = 0;	buf[2][1] = 0;	buf[1][2] = 0;
			break;
		case 2:
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 0;	buf[0][1] = 1;	buf[1][1] = 1;	buf[2][1] = 0;	buf[1][2] = 1;
			break;
		case 3:
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 0;	buf[2][1] = 0;	buf[1][2] = 1;
			break;
		case 4:
			buf[3][1] = 0;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 0;	buf[1][1] = 0;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 5:
			buf[3][1] = 1;	buf[2][2] = 0;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 0;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 6:
			buf[3][1] = 1;	buf[2][2] = 0;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 7:
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 0;	buf[1][1] = 0;	buf[2][1] = 0;	buf[1][2] = 0;
			break;
		case 8:
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 9:
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 1;	buf[0][1] = 1;	buf[1][1] = 0;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 10:
			buf[3][1] = 0;	buf[2][2] = 0;	buf[0][2] = 0;	buf[0][1] = 0;	buf[1][1] = 0;	buf[2][1] = 0;	buf[1][2] = 0;
			break;
		case 13://"D",实际显示为“P”，意为电源（POWER）类故障
			buf[3][1] = 1;	buf[2][2] = 1;	buf[0][2] = 0;	buf[0][1] = 0;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 14://"E"
			buf[3][1] = 1;	buf[2][2] = 0;	buf[0][2] = 0;	buf[0][1] = 1;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		case 15://"F"
			buf[3][1] = 1;	buf[2][2] = 0;	buf[0][2] = 0;	buf[0][1] = 0;	buf[1][1] = 1;	buf[2][1] = 1;	buf[1][2] = 1;
			break;
		default:
			break;	
	}
}

void Set_LCD_Num5(unsigned char num, unsigned char buf[4][13])			//底部2个7段数码管中第二个数码管
{
	switch(num)
	{
		case 0:// 5a  			5b				5c				5d				5e				5f				5g				
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 1;	buf[2][3] = 1;	buf[1][4] = 0;
			break;
		case 1:
			buf[3][3] = 0;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 0;	buf[1][3] = 0;	buf[2][3] = 0;	buf[1][4] = 0;
			break;
		case 2:
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 0;	buf[0][3] = 1;	buf[1][3] = 1;	buf[2][3] = 0;	buf[1][4] = 1;
			break;
		case 3:
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 0;	buf[2][3] = 0;	buf[1][4] = 1;
			break;
		case 4:
			buf[3][3] = 0;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 0;	buf[1][3] = 0;	buf[2][3] = 1;	buf[1][4] = 1;
			break;
		case 5:
			buf[3][3] = 1;	buf[2][4] = 0;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 0;	buf[2][3] = 1;	buf[1][4] = 1;
			break;
		case 6:
			buf[3][3] = 1;	buf[2][4] = 0;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 1;	buf[2][3] = 1;	buf[1][4] = 1;
			break;
		case 7:
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 0;	buf[1][3] = 0;	buf[2][3] = 0;	buf[1][4] = 0;
			break;
		case 8:
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 1;	buf[2][3] = 1;	buf[1][4] = 1;
			break;
		case 9:
			buf[3][3] = 1;	buf[2][4] = 1;	buf[0][4] = 1;	buf[0][3] = 1;	buf[1][3] = 0;	buf[2][3] = 1;	buf[1][4] = 1;
			break;
		case 10:
			buf[3][3] = 0;	buf[2][4] = 0;	buf[0][4] = 0;	buf[0][3] = 0;	buf[1][3] = 0;	buf[2][3] = 0;	buf[1][4] = 0;
		default:
			break;	
	}
}

void Set_LCD_BMP1(unsigned char mode, unsigned char ptc, unsigned char freshLv, unsigned char buf[4][13])	//顶部一排图标
{
	switch(mode)
	{
		case 0:
			buf[3][12] = 0;	buf[2][12] = 0;	buf[1][12] = 0;	buf[0][12] = 0;
			break;
		case 1:
			buf[3][12] = 1;	buf[2][12] = 0;	buf[1][12] = 0;	buf[0][12] = 0;
			break;
		case 2:
			buf[3][12] = 0;	buf[2][12] = 1;	buf[1][12] = 0;	buf[0][12] = 0;
			break;
		case 3:
			buf[3][12] = 0;	buf[2][12] = 0;	buf[1][12] = 1;	buf[0][12] = 0;
			break;
		case 4:
			buf[3][12] = 0;	buf[2][12] = 0;	buf[1][12] = 0;	buf[0][12] = 1;
			break;
		case 5:
			buf[3][12] = 1;	buf[2][12] = 1;	buf[1][12] = 1;	buf[0][12] = 1;
			break;
		case 6:
			buf[3][12] = 1;	buf[2][12] = 1;	buf[1][12] = 1;	buf[0][12] = 1;
			break;
		default:
			break;
	}
	if(ptc == 0)
		buf[0][11] = 0;
	else
		buf[0][11] = 1;
	if(freshLv == 0)
	{
		buf[1][11] = 0;
		buf[2][11] = 0;
		buf[3][11] = 0;
	}
	else if(freshLv == 1)
	{
		buf[1][11] = 1;
		buf[2][11] = 0;
		buf[3][11] = 0;
	}
	else
	{
		buf[1][11] = 1;
		buf[2][11] = 1;
		buf[3][11] = 1;
	}	
}

void Set_LCD_BMP2(unsigned char num, unsigned char buf[4][13])			//两侧风量图标
{
	switch(num)
	{
		case 0:
			buf[0][0] = 0;	buf[1][0] = 0;	buf[2][0] = 0;	buf[3][0] = 0;	buf[3][2] = 0;	buf[3][9] = 0;
			break;
		case 1:
			buf[0][0] = 1;	buf[1][0] = 0;	buf[2][0] = 0;	buf[3][0] = 0;	buf[3][2] = 0;	buf[3][9] = 0;
			break;
		case 2:
			buf[0][0] = 1;	buf[1][0] = 1;	buf[2][0] = 0;	buf[3][0] = 0;	buf[3][2] = 0;	buf[3][9] = 0;
			break;
		case 3:
			buf[0][0] = 1;	buf[1][0] = 1;	buf[2][0] = 1;	buf[3][0] = 0;	buf[3][2] = 0;	buf[3][9] = 0;
			break;
		case 4:
			buf[0][0] = 1;	buf[1][0] = 1;	buf[2][0] = 1;	buf[3][0] = 1;	buf[3][2] = 0;	buf[3][9] = 0;
			break;
		case 5:
			buf[0][0] = 1;	buf[1][0] = 1;	buf[2][0] = 1;	buf[3][0] = 1;	buf[3][2] = 1;	buf[3][9] = 0;
			break;
		case 6:
			buf[0][0] = 1;	buf[1][0] = 1;	buf[2][0] = 1;	buf[3][0] = 1;	buf[3][2] = 1;	buf[3][9] = 1;
			break;
		default:
			break;	
	}
}

void Handle_DisWindLv(unsigned char windlv, unsigned char buf[4][13])
{
	Set_LCD_BMP2(windlv, buf);
}

void Handle_DisModeInfo(unsigned char mode, unsigned char ptc, unsigned char freshLv, unsigned char buf[4][13])
{
	Set_LCD_BMP1(mode, ptc, freshLv, buf);
}

void Handle_DisSysTemper(unsigned char data_sign, float sys_temp, unsigned char buf[][13])
{
	unsigned char data_shi = 0;
	unsigned char data_ge = 0;
	unsigned char data_dot = 0;
	data_shi = (unsigned char)(((unsigned short)(sys_temp * 10))/100);
	data_ge  = (unsigned char)(((unsigned short)(sys_temp * 10))/10%10);
	data_dot = (unsigned char)(((unsigned short)(sys_temp * 10))%10);
	
	if(data_sign == 1)
	{
		Set_LCD_Num1(10,buf);
		Set_LCD_Num2(data_shi,buf);
		Set_LCD_Num3(data_ge,buf);
	}
	else
	{
		Set_LCD_Num1(data_shi,buf);
		Set_LCD_Num2(data_ge,buf);
		Set_LCD_Num3(data_dot,buf);
	}
}

void Handle_DisUserTemper(unsigned char usertemp, unsigned char buf[4][13])
{
	unsigned char data_shi = usertemp / 10;
	unsigned char data_ge =  usertemp % 10;
	
	Set_LCD_Num4(data_shi,buf);
	Set_LCD_Num5(data_ge,buf);
}

void Handle_DisErrNum(unsigned char *err_num, unsigned char buf[4][13])
{
	static unsigned char sDisNext_flag = 1;
	static unsigned char sErr_Index = 0;
	static unsigned char sDat1 = 0;
	static unsigned char sDat2 = 0;
	
	if((gBlinkTime == 0) && (sDisNext_flag == 1))
	{
		sDisNext_flag = 0;
		if(err_num[sErr_Index] != 0)
		{
			sDat1 = (err_num[sErr_Index] >> 4) & 0x0f;
			sDat2 = err_num[sErr_Index] & 0x0f;
			sErr_Index++;
		}
		else
		{
			sErr_Index = 0;
			sDat1 = (err_num[sErr_Index] >> 4) & 0x0f;
			sDat2 = err_num[sErr_Index] & 0x0f;
			sErr_Index++;
		}
	}
	if(gBlinkTime == BLINK_FRQ_TIME)
		sDisNext_flag = 1;
	
	Set_LCD_Num1(ERR_E,buf);
	Set_LCD_Num2(ERR_R,buf);
	Set_LCD_Num3(ERR_R,buf);
	Set_LCD_Num4(sDat1, buf);
	Set_LCD_Num5(sDat2,buf);
}

void Get_Err_Number(unsigned char *num_buf)
{
	unsigned char i = 0;
	unsigned char k = 0;
	unsigned char err_num[21] = {0};
	
	if(gSys_Info.ErrPower.bits.TravSwitch == 1)
		err_num[0] = 0xd1;						//"P1"行程开关故障
	else
		err_num[0] = 0;
	if(gSys_Info.ErrPower.bits.PowerOver == 1)
		err_num[1] = 0xd2;						//"P2"总电过压故障
	else
		err_num[1] = 0;
	if(gSys_Info.ErrPower.bits.PowerLow == 1)
		err_num[2] = 0xd3;						//"P3"总电欠压故障
	else
		err_num[2] = 0;
	if(gSys_Info.ErrPower.bits.DCDCOver == 1)
		err_num[3] = 0xd4;						//"P4"DCDC过压故障
	else
		err_num[3] = 0;
	if(gSys_Info.ErrPower.bits.DCDCLow == 1)
		err_num[4] = 0xd5;						//"P5"DCDC欠压故障
	else
		err_num[4] = 0;
	
	if(gSys_Info.ErrTempers.bits.BusIn == 1)
		err_num[5] = 0xe1;						//"E1"车内回风温度传感故障
	else
		err_num[5] = 0;
	if(gSys_Info.ErrTempers.bits.BusOut == 1)
		err_num[6] = 0xe2;						//"E2"车外环境温度传感故障
	else
		err_num[6] = 0;
	if(gSys_Info.ErrTempers.bits.InPipe == 1)
		err_num[9] = 0xe3;						//"E3"内盘管温度传感故障
	else
		err_num[9] = 0;
	if(gSys_Info.ErrTempers.bits.OutPipe == 1)
		err_num[10] = 0xe4;						//"E4"外盘管温度传感故障
	else
		err_num[10] = 0;
	if(gSys_Info.ErrTempers.bits.Store == 1)
		err_num[11] = 0xe5;						//"E5"电气仓温度传感故障
	else
		err_num[11] = 0;
	if(gSys_Info.ErrTempers.bits.CompHeat == 1)
		err_num[12] = 0xe6;						//"E6"压缩机过热故障
	else
		err_num[12] = 0;
	
	if(gSys_Info.Errorflags.bits.CompOverPres == 1)
		err_num[13] = 0xf1;						//"F1"压缩机过压传感故障
	else
		err_num[13] = 0;	
	if(gSys_Info.Errorflags.bits.CompLowPres == 1)
		err_num[14] = 0xf2;						//"F2"压缩机欠压传感故障
	else
		err_num[14] = 0;
	if(gSys_Info.Errorflags.bits.VFDFault == 1)
		err_num[15] = 0xf3;						//"F3"变频器故障
	else
		err_num[15] = 0;
	if(gSys_Info.Errorflags.bits.InnerFan == 1)
		err_num[16] = 0xf4;						//"F4"内机风机故障
	else
		err_num[16] = 0;
	if(gSys_Info.Errorflags.bits.OutsideFan == 1)
		err_num[17] = 0xf5;						//"F5"外机风机故障
	else
		err_num[17] = 0;
	if(gSys_Info.Errorflags.bits.CANFault == 1)
		err_num[18] = 0xf6;						//"F6"CAN通信故障
	else
		err_num[18] = 0;
	if(gSys_Info.Errorflags.bits.LeftFresh == 1)
		err_num[19] = 0xf7;						//"F7"左新风故障
	else
		err_num[19] = 0;
	if(gSys_Info.Errorflags.bits.RightFresh == 1)
		err_num[20] = 0xf8;						//"F7"右新风故障
	else
		err_num[20] = 0;

	k = 0;
	for(i=0; i<21; i++)				//将所有错误码存入数组返回
	{
		if(err_num[i] != 0)
		{
			num_buf[k] = err_num[i];
			k++;
		}
	}
}

void Get_Skim_Temper(unsigned char index, unsigned char *temper_sign, float *temper_data)
{
	switch(index)
	{
		case 1:
			*temper_sign = gSys_Info.BusInTmpSign;
			*temper_data = gSys_Info.BusInTmp;
			break;
		case 2:
			*temper_sign = gSys_Info.BusOutTmpSign;
			*temper_data = gSys_Info.BusOutTmp;
			break;
		case 3:
			*temper_sign = gSys_Info.InPipeTmpSign;
			*temper_data = gSys_Info.InPipeTmp;
			break;
		case 4:
			*temper_sign = gSys_Info.OutPipeTmpSign;
			*temper_data = gSys_Info.OutPipeTmp;
			break;
		case 5:
			*temper_sign = gSys_Info.StoreTmpSign;
			*temper_data = gSys_Info.StoreTmp;
			break;
		case 6:
			*temper_sign = gSys_Info.CompTmpSign;
			*temper_data = gSys_Info.CompTmp;
			break;
		default:
			break;
	}
}

void Handle_DisSkimData(unsigned char index, unsigned char temper_sign, float temper_data, unsigned char buf[][13])
{
	unsigned char data_shi = 0;
	unsigned char data_ge = 0;
	unsigned char data_dot = 0;
	data_shi = (unsigned char)(((unsigned short)(temper_data * 10))/100);
	data_ge  = (unsigned char)(((unsigned short)(temper_data * 10))/10%10);
	data_dot = (unsigned char)(((unsigned short)(temper_data * 10))%10);
	
	if(temper_sign == 1)
	{
		Set_LCD_Num1(10,buf);
		Set_LCD_Num2(data_shi,buf);
		Set_LCD_Num3(data_ge,buf);
	}
	else
	{
		Set_LCD_Num1(data_shi,buf);
		Set_LCD_Num2(data_ge,buf);
		Set_LCD_Num3(data_dot,buf);
	}
	
	data_shi = index / 10;
	data_ge  = index % 10;
	Set_LCD_Num4(data_shi,buf);
	Set_LCD_Num5(data_ge,buf);
}

void Handle_DisBlinkBmp(unsigned char buf[][13])
{
	unsigned char Bmp1 = 0;
	unsigned char Bmp2 = 0;

	if(gBlinkTime == (2 * BLINK_FRQ_TIME))
	{
		gBlinkTime = 0;
		Bmp1 = BMP_BLINK_ON;
		Bmp2 = BMP_BLINK_ON;
		Set_LCD_BMP1(Bmp1, Bmp1, Bmp1, buf);
		Set_LCD_BMP2(Bmp2, buf);
		HT1621_RAM_Update(buf);
	}
	if(gBlinkTime == BLINK_FRQ_TIME)
	{
		Bmp1 = BMP_BLINK_OFF;
		Bmp2 = BMP_BLINK_OFF;
		Set_LCD_BMP1(Bmp1, Bmp1, Bmp1, buf);
		Set_LCD_BMP2(Bmp2, buf);
		HT1621_RAM_Update(buf);
	}
}
/*
void Display_Test(void)
{
	unsigned char HT1621_RAM[4][13];
	unsigned int i=0;

	for(i=0; i<7; i++)
	{
		Handle_DisWindLv(i, HT1621_RAM);
		HT1621_RAM_Update(HT1621_RAM);
		Dis_Delay(DELAY_TEST_TIME);
	}

	for(i=0; i<9; i++)
	{
		Handle_DisUserTemper((i*10+i), HT1621_RAM);
		Handle_DisSysTemper(0, (0.1*(i*100+i*10+i)), HT1621_RAM);
		HT1621_RAM_Update(HT1621_RAM);
		Dis_Delay(DELAY_TEST_TIME);
	}
	
	gUser_Info->Mode = 2;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	gUser_Info->Mode = 3;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	gUser_Info->Mode = 4;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	gUser_Info->Mode = 1;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	gUser_Info->Mode = 7;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	gUser_Info->PTC = 1;
	Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
	HT1621_RAM_Update(HT1621_RAM);
	Dis_Delay(DELAY_TEST_TIME);
	
	for(i=0; i<3; i++)
	{
		gUser_Info->FreshAir = i;
		Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, HT1621_RAM);
		HT1621_RAM_Update(HT1621_RAM);
		Dis_Delay(DELAY_TEST_TIME);
	}
	for(i=0; i<20; i++)
		Dis_Delay(DELAY_TEST_TIME);
}
*/
void Display(void)
{
	unsigned int i=0;
	unsigned char j=0;
	unsigned char err_buf[21] = {0};
	unsigned char temp_sign = 0;
	float temp_data = 0.0;

	if(gUser_Info->Power == POWER_ON)			//上电后屏幕显示，断电时黑屏
	{
		if((gSys_Info.Errorflags.Byte != 0) || (gSys_Info.ErrTempers.Byte != 0) || (gSys_Info.ErrPower.Byte != 0))//系统有故障时显示故障信息
		{
			Get_Err_Number(err_buf);
			Handle_DisErrNum(err_buf, gHT1621_RAM);
		}
		else					//正常状态显示
		{
			if(gSkimFlag == 1)	//用户浏览系统信息
			{
				Get_Skim_Temper(gSkimIndex, &temp_sign, &temp_data);
				Handle_DisSkimData(gSkimIndex, temp_sign, temp_data, gHT1621_RAM);
			}
			else				//系统正常运行时显示
			{
				Handle_DisWindLv(gUser_Info->WindLv, gHT1621_RAM);
				Handle_DisUserTemper(gUser_Info->Temper, gHT1621_RAM);
				Handle_DisSysTemper(gSys_Info.BusInTmpSign, gSys_Info.BusInTmp, gHT1621_RAM);
				Handle_DisModeInfo(gUser_Info->Mode, gUser_Info->PTC, gUser_Info->FreshAir, gHT1621_RAM);
			}
		}
	}
	else
	{
		for(i=0; i<4; i++)
			for(j=0; j<13; j++)
				gHT1621_RAM[i][j] = 0;
	}
	HT1621_RAM_Update(gHT1621_RAM);	//更新HT1629内部RAM数据
}
