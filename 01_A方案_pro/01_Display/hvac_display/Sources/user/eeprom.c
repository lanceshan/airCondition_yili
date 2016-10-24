/*
 * eeprom.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "eeprom.h"

void Eeprom_Delay(uint16 cnt)
{
	while(cnt--);
}

uint8 Eeprom_Wr(uint16 addr, uint8 *buf, uint8 len)
{
	uint8 i=0;
	uint8 status=0;
	for(i=0; i<len; i++)
	{
		IIC_Init();
		status |= IIC_Start();
		I2C1_C1 |= I2C_C1_TX_MASK;
		status |= IIC_WriteOneByte(0xa0);						//发送从机地址写操作，1010 000 0
		status |= IIC_WriteOneByte((uint8)((addr>>8)&0x00ff));	//发送地址高8位
		status |= IIC_WriteOneByte((uint8)(addr&0x00ff));		//发送地址低8位
		status |= IIC_WriteOneByte(buf[i]);						//发送数据
		status |= IIC_Stop();
		Eeprom_Delay(6000);										//连续写之间短暂延时
		addr++;
	}
	return status;
}

uint8 Eeprom_Rd(uint16 addr, uint8 *buf, uint8 len)
{
	uint8 status=0;
	uint8 i=0;
	
	for(i=0; i<len; i++)
	{
		IIC_Init();
		status |= IIC_Start();
		I2C1_C1 |= I2C_C1_TX_MASK;
		status |= IIC_WriteOneByte(0xa0);				//发送从机地址写操作，1010 000 1
		status |= IIC_WriteOneByte((addr>>8)&0xff);		//发送地址高8位
		status |= IIC_WriteOneByte((addr&0xff));		//发送地址低8位		
		
		IIC_Init();
		status |= IIC_Start();
		status |= IIC_WriteOneByte(0xa1);
		
		buf[i] = IIC_ReadOneByte(I2C_SEND_NACK);		//假读
		buf[i] = IIC_ReadOneByte(I2C_SEND_NACK);

		status |= IIC_Stop();
		Eeprom_Delay(3000);
		addr++;
	}
	return i;
}

uint8 Read_Prv_Info(uint8 *UserBuf, uint8 len)
{
	uint8 rev_len = 0;
	uint8 buf[8] = {0};
	uint8 i = 0;
	
	while((rev_len != len) && (i++ < 10))	//读取失败，反复读，读10次失败后，放弃
		rev_len = Eeprom_Rd(0, buf, len);
	if(rev_len == len)
	{
		for(i=0; i<len; i++)
			UserBuf[i] = buf[i];
	}
	if((UserBuf[0]>32) || (UserBuf[0]<16))UserBuf[0] = 25;		//限制用户温度数据范围，16~32
	if((UserBuf[1]> 6) || (UserBuf[1]< 1))UserBuf[1] = 3;		//限制用户设置模式
	if((UserBuf[2]> 6) || (UserBuf[2]< 1))UserBuf[2] = 3;		//限制用户设置风量，1~6
	return rev_len;
}

void Save_Cur_Info(uint8 *Buf, uint8 len)
{
	Eeprom_Wr(0, Buf, len);
}



