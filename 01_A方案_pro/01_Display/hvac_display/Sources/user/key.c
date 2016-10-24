/*
 * key.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "key.h"
#include "gpio.h"
#include "commprotocol.h"

#define KEY_JITTER_DELAY	3000	//������ʱ����

void Key_Delay(uint16 k)
{
	uint16 i=0;
	uint16 j=0;
	for(i=k; i>0; i--);
		for(j=1000; j>0; j--);
}

KEY_STATE Key_Scan(void)			//����ɨ��
{
	KEY_STATE key_status = Key_Null;

	if(Get_KEY_POWER == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_POWER == 0)
		{
			key_status = Key_Power;	
		}
	}	
	else if(Get_KEY_MODE == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_MODE == 0)
		{
			key_status = Key_Mode;
		}	
	}
	else if(Get_KEY_AUTO == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_AUTO == 0)
		{
			key_status = Key_Auto;				
		}
	}
	else if(Get_KEY_PTC == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_PTC == 0)
		{
			key_status = Key_PTC;
		}
	}
	else if(Get_KEY_FRESHFAN == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_FRESHFAN == 0)
		{
			key_status = Key_FreshAir;	
		}
	}
	else if(Get_KEY_SKIM == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_SKIM == 0)
		{
			key_status = Key_Skim;		
		}	
	}
	else if(Get_KEY_TEMP_PLUS == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_TEMP_PLUS == 0)
		{
			key_status = Key_TemperPlus;	
		}			
	}
	else if(Get_KEY_TEMP_MINUS == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_TEMP_MINUS == 0)
		{
			key_status = Key_TemperMinus;
		}					
	}
	else if(Get_KEY_WIND_PLUS == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_WIND_PLUS == 0)
		{
			key_status = Key_WindPlus;
		}
	}
	else if(Get_KEY_WIND_MINUS == 0)
	{
		Key_Delay(KEY_JITTER_DELAY);
		if(Get_KEY_WIND_MINUS == 0)
		{
			key_status = Key_WindMinus;	
		}			
	}
	gKeyStatus = key_status;		//�˴�����ǰ����״̬��gKeyStatus��Ϊ���ڶ�ʱ�ж���ȷ��������������		
	return key_status;
}


unsigned char Key_Handle(void)		//���������Կ��Ʊ��������޸�
{	
	unsigned char new_cmd_flag = NEW_CMD_NULL;
	unsigned char temp_buf[8] = {0};
	KEY_STATE status = Key_Null;
	
	status = Key_Scan();

	if(status == Key_Null)
	{
		gKeyPresTime = 0;
		return new_cmd_flag;		//�ް������£�������������ֱ�ӷ���
	}
	if(status != Key_Power)			
	{
		if(gUser_Info->Power == POWER_OFF)
			return new_cmd_flag;	//�ڹػ�״̬�£����·ǵ�Դ����������������ֱ�ӷ���
	}
	
	Buzzer();						//����״̬�°��°�������������
	
	switch (status)
	{
		case Key_Power:
			while(Key_Scan() != Key_Null);	//��Դ���������ֺ�ϵͳ�������߹ر�
			gUser_Info->Power ^= 0x01;
			if(gUser_Info->Power == POWER_OFF)	
			{
				gUser_Info->FreshAir = FRESH_FAN_OFF;
				gUser_Info->PTC = 0;
				gSys_Info.Errorflags.Byte = 0;
				gSys_Info.ErrPower.Byte = 0;
				gSys_Info.ErrTempers.Byte = 0;
				gSkimTime = 0;
				gSkimFlag = 0;
				gSkimIndex = 0;
			}
			new_cmd_flag = NEW_CMD_FAST;	//��Դ��Ϊ������Ӧ����
			break;
		case Key_WindMinus:
			if(gSkimFlag != 0)				//��ѯģʽʱ�������������л���ѯ��Ϣ
			{
				gSkimIndex = (gSkimIndex + (SKIM_TEMPER_NUM - 2)) % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if(gUser_Info->Mode != MODE_AUTO)	//�Զ�ģʽ�²��ܵ��ڷ���
					if(gUser_Info->WindLv > MIN_BLOWER_Lv)
					{
						gUser_Info->WindLv--;
						new_cmd_flag = NEW_CMD_FAST;	//������Ϊ������Ӧ����
					}
			}
			break;
		case Key_WindPlus:
			if(gSkimFlag != 0)
			{
				gSkimIndex = gSkimIndex % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if(gUser_Info->Mode != MODE_AUTO)
					if(gUser_Info->WindLv < MAX_BLOWER_Lv)
					{
						gUser_Info->WindLv++;
						new_cmd_flag = NEW_CMD_FAST;
					}
			}
			break;
		case Key_TemperMinus:
			if(gSkimFlag != 0)				//��ѯģʽʱ���¿ؼ������л���ѯ��Ϣ
			{
				gSkimIndex = (gSkimIndex + (SKIM_TEMPER_NUM - 2)) % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if((gUser_Info->Mode != MODE_AUTO) && (gUser_Info->Mode != MODE_BLOW))	//�Զ�ģʽ�ʹ���ģʽ���ܵ��¶�
				{
					if(gUser_Info->Temper > MIN_SET_TEMPER)
					{
						gUser_Info->Temper--;
						new_cmd_flag = NEW_CMD_SLOW;	//�¿ؼ�Ϊ������Ӧ����
					}
				}
			}
			break;
		case Key_TemperPlus:
			if(gSkimFlag != 0)
			{
				gSkimIndex = gSkimIndex % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if((gUser_Info->Mode != MODE_AUTO) && (gUser_Info->Mode != MODE_BLOW))//�Զ�ģʽ�ʹ���ģʽ���ܵ����¶�
				{
					if(gUser_Info->Temper < MAX_SET_TEMPER)
					{
						gUser_Info->Temper++;
						new_cmd_flag = NEW_CMD_SLOW;
					}
				}
			}
			break;
		case Key_Auto:
			if(gSkimFlag != 0)			//��ѯģʽʱ������AUTO�����ظ�������ʾ
			{
				gSkimFlag = 0;
				gSkimTime = 0;
			}
			else
			{
				gUser_Info->Mode = MODE_AUTO;
				new_cmd_flag = NEW_CMD_SLOW;
			}
			break;
		case Key_Mode:
			if(gSkimFlag != 0)
			{
				gSkimFlag = 0;
				gSkimTime = 0;
			}
			else
			{
				gUser_Info->Mode = (gUser_Info->Mode % 4) + 1;//�����䡢���ȡ�ͨ�硢�Զ�ģʽѭ���л�
				new_cmd_flag = NEW_CMD_SLOW;
			}
			break;	
		case Key_FreshAir:
			if(gSkimFlag != 0)
			{
				gSkimFlag = 0;
				gSkimTime = 0;
			}
			else
			{
				switch(gUser_Info->FreshAir)
				{
					case FRESH_FAN_OFF:	
						gUser_Info->FreshAir = FRESH_FAN_LOW;	//��һ�ΰ��£��·�뿪
						break;
					case FRESH_FAN_LOW:
						gUser_Info->FreshAir = FRESH_FAN_HIGH;	//�ڶ��ΰ��£��·�ȫ��
						break;
					case FRESH_FAN_HIGH:
						gUser_Info->FreshAir = FRESH_FAN_OFF;	//�����ΰ��£��·�ر�
						break;
					default:
						break;
				}
				new_cmd_flag = NEW_CMD_SLOW;
			}
			break;
		case Key_PTC:
			if(gSkimFlag != 0)
			{
				gSkimFlag = 0;
				gSkimTime = 0;
			}
			else
			{
				gUser_Info->PTC ^= 0x01;
				new_cmd_flag = NEW_CMD_SLOW;
			}
			break;
		case Key_Skim:								//��ѯϵͳ��Ϣ
			if(gKeyPresHold.bits.Skim == 1)
			{		
				gSkimFlag = 1;						//��ѯ���������£�������ѯģʽ
				gSkimTime = SKIM_TIME;
			}
			else
			{
				gSkimFlag = 0;
				gSkimTime = 0;
			}
			gSkimIndex = 1;
			break;
		default:
			break;
	}	
	gKeyHandleFlag = 0;
	while((Key_Scan() != Key_Null) && (gKeyHandleFlag == 0))			//���ֵȴ�,����ʱ��400ms������һ�ΰ�������
	{
		if(8 == CAN_Rx())CAN_Tx(temp_buf);								//��������ʱ������CAN�շ��������ڰ���������ͨ���쳣������ϵͳ����
	}
	return new_cmd_flag;
}

