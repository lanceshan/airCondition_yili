/*
 * globalvars.c
 *
 *  Created on: Oct 28, 2015
 *      Author: Hill
 */
#include "globalvars.h"

uint8 gBlinkTime;			//查询系统中各温度时，风量图标闪烁时间间隔
uint8 gSkimIndex;			//查询系统中各温度时，依次查询索引号
uint8 gSkimFlag;			//用户浏览系统内温度标志
uint8 gSkimTime;			//查询状态持续时间
uint8 gSetOKTime;			//按键操作完成时间
uint8 gKeyPresTime;			//按键按下时间
uint8 gKeyHandleFlag;		//按键处理标志，用在长按时，每400ms执行下次按键处理
uint8 gCANRxFailTime;		//CAN通信失败计时
uint8 gHT1621_RAM[4][13];
SysMessage gSys_Info;		//接收驱动板的系统信息
UserMessage *gUser_Info;	//用户设置信息
KeyPresHold gKeyPresHold;	//记录所有按键长按的状态
KEY_STATE gKeyStatus;		//实时记录当前按下的按键

void Var_Init(void)
{
	unsigned int i=0;
	unsigned char j=0;
	
	for(i=0; i<4; i++)
		for(j=0; j<13; j++)
			gHT1621_RAM[i][j] = 0;			//系统上电时清掉显示数据
	
	gBlinkTime  = 0;
	gSkimIndex  = 0;
	gSkimFlag   = 0;
	gSkimTime   = 0;
	gSetOKTime  = 0;
	gKeyStatus  = 0;
	gUser_Info->cmd      = 0x33;			//发送用户指令命令码
	gUser_Info->Temper   = 18;				//系统出厂默认设定温度18℃
	gUser_Info->Mode     = MODE_BLOW;			
	gUser_Info->WindLv   = 1;				//默认风量1级
	gUser_Info->FreshAir = FRESH_FAN_OFF;	//默认新风关闭状态
	gUser_Info->TestIndex= 0;
	gUser_Info->Power    = POWER_OFF;		//默认电源关闭状态
	gUser_Info->PTC      = 0;				//PTC关闭
	
	gSys_Info.BusInTmp   = 25.0;
	gSys_Info.InPipeTmp  = 0.0;
	gSys_Info.OutPipeTmp = 0.0;
}
