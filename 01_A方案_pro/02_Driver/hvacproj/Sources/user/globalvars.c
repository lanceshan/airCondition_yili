/*
 * globalvars.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#include "globalvars.h"
//通信相关
unsigned char CANRx_Buf[8]={0};				//CAN接收缓存
unsigned char CANTx_Buf[8]={0};				//CAN发送缓存
unsigned long CAN_ID = 0;

//定时器相关
unsigned int  gSysStartTcnt = 0;			//系统上电启动5s定时计数
unsigned int  gMsgTxTcnt = 0; 				//CAN信息发送时间间隔500ms
unsigned int  gSysRunTcnt = TIME_SEC; 		//系统运行间隔1s定时计数
unsigned int  gColdToHotTcnt = 0;			//制冷切换到制热模式延时3min定时计数
unsigned int  gHotToColdTcnt = 0;			//制热切换到制冷模式延时1min定时计数
unsigned int  gGetSysInfoTcnt = 0;			//采集温度时间间隔
unsigned int  gTrendChgTcnt = 0;			//判断温度变化趋势时间间隔5s
unsigned int  gOFFModeTcnt = 0;				//关机模式计时
unsigned int  gDCErrCheckTcnt = 0;			//DCDC故障延时检测时间
unsigned int  gPowerErrCheckTcnt = 0;		//总电故障延时检测时间
unsigned int  gDefrostTcnt = 0;				//除霜状态计时
unsigned int  gFrostLowTempTcnt = 0;		//除霜温度传感器低温计时
unsigned int  gRightFreshWks_Tcnt = 0;		//新风门电机工作时间
unsigned int  gLeftFreshWks_Tcnt = 0;
DRV_STATE 	  gcur_state = state_halt;
UserMessage   user_Info;
SysMessage    sys_Info;
CtrlMessage   sys_Ctrl;
