/*
 * error.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "error.h"
#include "globalvars.h"
#include "mode.h"
#include "tactics.h"
#include "CompLowPress_dFault.h"
#include "CompOverPress_dFault.h"
#include "InnerFan_dFault.h"
#include "OutsideFan_dFault.h"
#include "TravelSwitch_dFault.h"
#include "PowerLow_dFault.h"
#include "PowerOver_dFault.h"
#include "VFDRun_dFault.h"
#include "VFDRun_dFlag.h"
#include "function\fan.h"
#include "function\compressor.h"
#include "function\ptc.h"
#include "function\valve.h"
#include "function\power.h"

unsigned char ErrorCheck(void)	//故障检测，故障消除后自动恢复
{
	uint8 error = ERR_NULL;
	if(gPowerErrCheckTcnt == 0)
	{	
		if(PowerOver_dFault_GetVal() == 0)		//总电高于800V
		{
			sys_Info.ErrPower.bits.PowerOver = 1;
			error |= ERR_Lv1;					//一级故障
		}
		else
		{
			sys_Info.ErrPower.bits.PowerOver = 0;
		}
		
		if(PowerLow_dFault_GetVal() == 0)		//总电低于450V,高低压接在一起，只检测高压故障
		{
			sys_Info.ErrPower.bits.PowerLow = 1;
//			error |= ERR_Lv1;					//一级故障
		}
		else
		{
			sys_Info.ErrPower.bits.PowerLow = 0;
		}
	}
	if(gDCErrCheckTcnt <= 0)					//上电10s后进行DCDC电压检测,避开DCDC上电没有输出时间段
	{
		if(sys_Info.DCDCVol > 30)				//DCDC输出过压
		{
			sys_Info.ErrPower.bits.DCDCOver = 1;
			error |= ERR_Lv1;					//一级故障
		}
		else if(sys_Info.DCDCVol < 28)
		{
			sys_Info.ErrPower.bits.DCDCOver = 0;
		}
		if(sys_Info.DCDCVol < 18)				//DCDC输出欠压
		{
			sys_Info.ErrPower.bits.DCDCLow = 1;
			error |= ERR_Lv1;					//一级故障
		}
		else if(sys_Info.DCDCVol > 20)
		{
			sys_Info.ErrPower.bits.DCDCLow = 0;
		}
	}

	if(TravelSwitch_dFault_GetVal() == 1)		//行程开关未关报警
	{
		sys_Info.ErrPower.bits.TravSwitch = 1;
		error |= ERR_Lv1;						//一级故障
	}
	else
	{
		sys_Info.ErrPower.bits.TravSwitch = 0;
	}
	
	if((sys_Info.StoreTmp >= 75) || (sys_Info.StoreTmpErrTimes >= 20))
	{											//控制仓高温或者传感器失效
		sys_Info.ErrTempers.bits.Store = 1;
		error |= ERR_Lv1;						//一级故障
	}
	else
	{
		sys_Info.ErrTempers.bits.Store = 0;
	}
	
	if(sys_Info.CANRecvErrTimes >= 10)			//CAN通信失败
	{
		sys_Info.ErrDevices.bits.CANFault = 1;
		error |= ERR_Lv1;						//一级故障
	}
	else
	{
		sys_Info.ErrDevices.bits.CANFault = 0;
	}
	
	if(VFDRun_dFault_GetVal() == 0)				//变频器故障
	{
		sys_Info.ErrDevices.bits.VFDFault = 1;
		error |= ERR_Lv2;						//二级故障
	}
	else
	{
		sys_Info.ErrDevices.bits.VFDFault = 0;
	}

	if(CompOverPress_dFault_GetVal() == 1)		//压缩机过压，高电平报错
	{
		sys_Info.ErrDevices.bits.CompOverPres = 1;
		error |= ERR_Lv2;						//二级故障
	}
	else
	{
		sys_Info.ErrDevices.bits.CompOverPres = 0;
	}
	
	if(CompLowPress_dFault_GetVal() == 1)		//压缩机欠压，高电平报错
	{
		sys_Info.ErrDevices.bits.CompLowPres = 1;
		error |= ERR_Lv2;						//二级故障
	}
	else
	{
		sys_Info.ErrDevices.bits.CompLowPres = 0;
	}
	
	if(sys_Info.CompTmpErrTimes >= 20)			//压缩机高温或传感器坏
	{
		sys_Info.ErrTempers.bits.CompHeat = 1;
		error |= ERR_Lv3;						//三级故障
	}
	else
	{
		sys_Info.ErrTempers.bits.CompHeat = 0;
	}
	
	if(sys_Info.BusInTmpErrTimes >= 20)			//车内温度传感器故障
	{
		sys_Info.ErrTempers.bits.BusIn = 1;
		error |= ERR_Lv3;						//三级故障
	}
	else
	{
		sys_Info.ErrTempers.bits.BusIn = 0;
	}

	if(sys_Info.BusOutTmpErrTimes >= 20)		//车外、内盘管、外盘管温度传感器故障，报警但是正常运行
		sys_Info.ErrTempers.bits.BusOut = 1;
	else
		sys_Info.ErrTempers.bits.BusOut = 0;
	
	if(sys_Info.InPipeTmpErrTimes >= 20)
		sys_Info.ErrTempers.bits.InPipe = 1;
	else
		sys_Info.ErrTempers.bits.InPipe = 0;
	
	if(sys_Info.OutPipeTmpErrTimes >= 20)
		sys_Info.ErrTempers.bits.OutPipe = 1;
	else
		sys_Info.ErrTempers.bits.OutPipe = 0;	

	return error;
}

void ErrorHandle(unsigned char err)
{
	if((err & ERR_Lv1) != 0)
	{
		user_Info.Power = 0;
		gDCErrCheckTcnt = TIME_SEC * 20;
		gPowerErrCheckTcnt = TIME_SEC * 60;		//关机后总电检测倒计时，避开DCDC断电后40s故障报错
		Power_OFF();
		return;
	}
	if((err & ERR_Lv2) != 0)
	{
		if(sys_Info.ErrDevices.bits.VFDFault == 1)
		{
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)sys_Ctrl.CompOnEn = 0;
			if(user_Info.Mode == MODE_COLD)
			{
				sys_Ctrl.InnerFanLv = user_Info.WindLv;
				sys_Ctrl.OutsideFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);			//蒸发风机设置
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			}
			if(user_Info.Mode == MODE_HOT)
			{
				sys_Ctrl.PTCLv = user_Info.PTC;
				sys_Ctrl.OutsideFanLv = 0;
				Set_PTC(sys_Ctrl.PTCLv);
				sys_Ctrl.InnerFanLv = Tactic_AutoHotInnerFanLv();
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			}
		}
		if((sys_Info.ErrDevices.bits.CompOverPres == 1) || (sys_Info.ErrDevices.bits.CompLowPres == 1) || (sys_Info.ErrTempers.bits.CompHeat == 1))
		{
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)
			{
				sys_Ctrl.CompOnEn = 0;	//压缩机1分钟以后允许打开
				Close_Valve();
			}
			if(user_Info.Mode == MODE_COLD)
			{
				sys_Ctrl.InnerFanLv = user_Info.WindLv;
				sys_Ctrl.OutsideFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);			//蒸发风机设置
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			}
			if(user_Info.Mode == MODE_HOT)
			{
				sys_Ctrl.PTCLv = user_Info.PTC;
				sys_Ctrl.OutsideFanLv = 0;
				Set_PTC(sys_Ctrl.PTCLv);
				sys_Ctrl.InnerFanLv = Tactic_AutoHotInnerFanLv();
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			}
		}
	}

	if((err & ERR_Lv3) != 0)
	{
		if(sys_Info.ErrTempers.bits.BusIn == 1)
		{
			sys_Info.BusInTmp = sys_Info.BusOutTmp;
			AutoModeProcess();
		}
	}
}

