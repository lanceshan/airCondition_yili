/*
 * ht1621.c
 *
 *  Created on: Apr 19, 2016
 *      Author: Hill
 */

#include "ht1621.h"

#define BIAS 0x52 	//0b1000 0101 0010 1/3duty 4com

#define SYSDIS 0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN 0X02 	//0b1000 0000 0010 打开系统振荡器

#define LCDOFF 0X04 //0b1000 0000 0100 关LCD偏压
#define LCDON 0X06 	//0b1000 0000 0110 打开LCD偏压
#define XTAL 0x28 	//0b1000 0010 1000 外部接时钟
#define RC256 0X30 	//0b1000 0011 0000 内部时钟
#define TONEON 0X12 //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10//0b1000 0001 0000 关闭声音输出
#define WDTDIS 0X0A //0b1000 0000 1010 禁止看门狗

#define DELAY_US_TIME 		3
#define DELAY_MS_TIME

static void Delayus(uint16 i)
{
	while(i--);
}

void Ht1621_WrData(unsigned char Dat, unsigned char cnt)
{
	uint8 i;
	for(i=0; i<cnt; i++)
	{
		HT1621_Set_WR_L;
		Delayus(DELAY_US_TIME);
		if((Dat & 0x80) == 0x80)
			HT1621_Set_DA_H;
		else
			HT1621_Set_DA_L;
		Delayus(DELAY_US_TIME);
		HT1621_Set_WR_H;
		Delayus(DELAY_US_TIME);
		Dat <<= 1;
	}
	
}
void Ht1621_WrCmd(unsigned char Cmd)
{
	HT1621_Set_CS_L;
	Delayus(DELAY_US_TIME);
	Ht1621_WrData(0x80,4);
	Ht1621_WrData(Cmd,8);
	HT1621_Set_CS_H;
	Delayus(DELAY_US_TIME);
}

void Ht1621_WrOneData(uint8 Data, uint8 Addr)
{
	HT1621_Set_CS_L;
	Ht1621_WrData(0xa0,3); //写入数据标志101
	Ht1621_WrData(Addr<<2,6); //写入地址数据
	Ht1621_WrData(Data<<4,4); //写入数据
	HT1621_Set_CS_H;
	Delayus(DELAY_US_TIME);
}

void Ht1621_ClearAllData(uint8 Addr,uint8 cnt)
{
	uint8 i;
	HT1621_Set_CS_L;
	Ht1621_WrData(0xa0,3); //写入数据标志101
	Ht1621_WrData(Addr<<2,6); //写入地址数据
	for (i=0;i<cnt;i++)
		Ht1621_WrData(0,8); //写入数据
	HT1621_Set_CS_H;
	Delayus(DELAY_US_TIME);
}

void HT1621_RAM_Update(unsigned char buf[][13])
{
	unsigned char i=0;
	unsigned char j=0;
	unsigned char dat[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
	for(j=0; j<13; j++)
	{
		for(i=0; i<4; i++)
		{
			if(buf[i][j] == 1)
			{
				dat[j] >>= 1;
				dat[j] |= 0x08;
			}
			else
			{
				dat[j] >>= 1;
			}
		}
		dat[j] &= 0x0f;
	}
	
	for(i=0; i<13; i++)
		Ht1621_WrOneData(dat[i],i);
}

void Ht1621_Init(void)
{
	Delayus(50000);
	HT1621_Set_CS_H;
	HT1621_Set_WR_H;
	HT1621_Set_DA_H;
	Delayus(50000);
	Ht1621_WrCmd(BIAS);
	Ht1621_WrCmd(RC256);
	Ht1621_WrCmd(SYSDIS);
	Ht1621_WrCmd(WDTDIS);
	Ht1621_WrCmd(SYSEN);
	Ht1621_WrCmd(LCDON);
	Ht1621_ClearAllData(0,16);
}
