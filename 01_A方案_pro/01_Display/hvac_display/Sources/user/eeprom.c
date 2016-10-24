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
		status |= IIC_WriteOneByte(0xa0);						//���ʹӻ���ַд������1010 000 0
		status |= IIC_WriteOneByte((uint8)((addr>>8)&0x00ff));	//���͵�ַ��8λ
		status |= IIC_WriteOneByte((uint8)(addr&0x00ff));		//���͵�ַ��8λ
		status |= IIC_WriteOneByte(buf[i]);						//��������
		status |= IIC_Stop();
		Eeprom_Delay(6000);										//����д֮�������ʱ
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
		status |= IIC_WriteOneByte(0xa0);				//���ʹӻ���ַд������1010 000 1
		status |= IIC_WriteOneByte((addr>>8)&0xff);		//���͵�ַ��8λ
		status |= IIC_WriteOneByte((addr&0xff));		//���͵�ַ��8λ		
		
		IIC_Init();
		status |= IIC_Start();
		status |= IIC_WriteOneByte(0xa1);
		
		buf[i] = IIC_ReadOneByte(I2C_SEND_NACK);		//�ٶ�
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
	
	while((rev_len != len) && (i++ < 10))	//��ȡʧ�ܣ�����������10��ʧ�ܺ󣬷���
		rev_len = Eeprom_Rd(0, buf, len);
	if(rev_len == len)
	{
		for(i=0; i<len; i++)
			UserBuf[i] = buf[i];
	}
	if((UserBuf[0]>32) || (UserBuf[0]<16))UserBuf[0] = 25;		//�����û��¶����ݷ�Χ��16~32
	if((UserBuf[1]> 6) || (UserBuf[1]< 1))UserBuf[1] = 3;		//�����û�����ģʽ
	if((UserBuf[2]> 6) || (UserBuf[2]< 1))UserBuf[2] = 3;		//�����û����÷�����1~6
	return rev_len;
}

void Save_Cur_Info(uint8 *Buf, uint8 len)
{
	Eeprom_Wr(0, Buf, len);
}



