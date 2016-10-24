/*
 * key.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "key.h"
#include "gpio.h"
#include "commprotocol.h"

#define KEY_JITTER_DELAY	3000	//消抖延时参数

void Key_Delay(uint16 k)
{
	uint16 i=0;
	uint16 j=0;
	for(i=k; i>0; i--);
		for(j=1000; j>0; j--);
}

KEY_STATE Key_Scan(void)			//按键扫描
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
	gKeyStatus = key_status;		//此处将当前按键状态给gKeyStatus，为了在定时中断中确定长按按键类型		
	return key_status;
}


unsigned char Key_Handle(void)		//按键处理，对控制变量进行修改
{	
	unsigned char new_cmd_flag = NEW_CMD_NULL;
	unsigned char temp_buf[8] = {0};
	KEY_STATE status = Key_Null;
	
	status = Key_Scan();

	if(status == Key_Null)
	{
		gKeyPresTime = 0;
		return new_cmd_flag;		//无按键按下，不做按键处理，直接返回
	}
	if(status != Key_Power)			
	{
		if(gUser_Info->Power == POWER_OFF)
			return new_cmd_flag;	//在关机状态下，按下非电源键，不做按键处理，直接返回
	}
	
	Buzzer();						//开机状态下按下按键，蜂鸣器响
	
	switch (status)
	{
		case Key_Power:
			while(Key_Scan() != Key_Null);	//电源键按下松手后系统启动或者关闭
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
			new_cmd_flag = NEW_CMD_FAST;	//电源键为快速响应按键
			break;
		case Key_WindMinus:
			if(gSkimFlag != 0)				//查询模式时，风量键用于切换查询信息
			{
				gSkimIndex = (gSkimIndex + (SKIM_TEMPER_NUM - 2)) % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if(gUser_Info->Mode != MODE_AUTO)	//自动模式下不能调节风量
					if(gUser_Info->WindLv > MIN_BLOWER_Lv)
					{
						gUser_Info->WindLv--;
						new_cmd_flag = NEW_CMD_FAST;	//风量键为快速响应按键
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
			if(gSkimFlag != 0)				//查询模式时，温控键用于切换查询信息
			{
				gSkimIndex = (gSkimIndex + (SKIM_TEMPER_NUM - 2)) % SKIM_TEMPER_NUM + 1;
				gSkimTime = SKIM_TIME;
			}
			else
			{
				if((gUser_Info->Mode != MODE_AUTO) && (gUser_Info->Mode != MODE_BLOW))	//自动模式和吹风模式不能调温度
				{
					if(gUser_Info->Temper > MIN_SET_TEMPER)
					{
						gUser_Info->Temper--;
						new_cmd_flag = NEW_CMD_SLOW;	//温控键为慢速响应按键
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
				if((gUser_Info->Mode != MODE_AUTO) && (gUser_Info->Mode != MODE_BLOW))//自动模式和吹风模式不能调节温度
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
			if(gSkimFlag != 0)			//查询模式时，按下AUTO键，回复正常显示
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
				gUser_Info->Mode = (gUser_Info->Mode % 4) + 1;//在制冷、制热、通风、自动模式循环切换
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
						gUser_Info->FreshAir = FRESH_FAN_LOW;	//第一次按下，新风半开
						break;
					case FRESH_FAN_LOW:
						gUser_Info->FreshAir = FRESH_FAN_HIGH;	//第二次按下，新风全开
						break;
					case FRESH_FAN_HIGH:
						gUser_Info->FreshAir = FRESH_FAN_OFF;	//第三次按下，新风关闭
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
		case Key_Skim:								//查询系统信息
			if(gKeyPresHold.bits.Skim == 1)
			{		
				gSkimFlag = 1;						//查询键被长按下，启动查询模式
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
	while((Key_Scan() != Key_Null) && (gKeyHandleFlag == 0))			//松手等待,长按时间400ms进入下一次按键处理
	{
		if(8 == CAN_Rx())CAN_Tx(temp_buf);								//按键长按时，进行CAN收发，避免在按键长按后通信异常，导致系统报错
	}
	return new_cmd_flag;
}

