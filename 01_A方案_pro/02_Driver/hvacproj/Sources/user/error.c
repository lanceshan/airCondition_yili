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

unsigned char ErrorCheck(void)	//���ϼ�⣬�����������Զ��ָ�
{
	uint8 error = ERR_NULL;
	if(gPowerErrCheckTcnt == 0)
	{	
		if(PowerOver_dFault_GetVal() == 0)		//�ܵ����800V
		{
			sys_Info.ErrPower.bits.PowerOver = 1;
			error |= ERR_Lv1;					//һ������
		}
		else
		{
			sys_Info.ErrPower.bits.PowerOver = 0;
		}
		
		if(PowerLow_dFault_GetVal() == 0)		//�ܵ����450V,�ߵ�ѹ����һ��ֻ����ѹ����
		{
			sys_Info.ErrPower.bits.PowerLow = 1;
//			error |= ERR_Lv1;					//һ������
		}
		else
		{
			sys_Info.ErrPower.bits.PowerLow = 0;
		}
	}
	if(gDCErrCheckTcnt <= 0)					//�ϵ�10s�����DCDC��ѹ���,�ܿ�DCDC�ϵ�û�����ʱ���
	{
		if(sys_Info.DCDCVol > 30)				//DCDC�����ѹ
		{
			sys_Info.ErrPower.bits.DCDCOver = 1;
			error |= ERR_Lv1;					//һ������
		}
		else if(sys_Info.DCDCVol < 28)
		{
			sys_Info.ErrPower.bits.DCDCOver = 0;
		}
		if(sys_Info.DCDCVol < 18)				//DCDC���Ƿѹ
		{
			sys_Info.ErrPower.bits.DCDCLow = 1;
			error |= ERR_Lv1;					//һ������
		}
		else if(sys_Info.DCDCVol > 20)
		{
			sys_Info.ErrPower.bits.DCDCLow = 0;
		}
	}

	if(TravelSwitch_dFault_GetVal() == 1)		//�г̿���δ�ر���
	{
		sys_Info.ErrPower.bits.TravSwitch = 1;
		error |= ERR_Lv1;						//һ������
	}
	else
	{
		sys_Info.ErrPower.bits.TravSwitch = 0;
	}
	
	if((sys_Info.StoreTmp >= 75) || (sys_Info.StoreTmpErrTimes >= 20))
	{											//���Ʋָ��»��ߴ�����ʧЧ
		sys_Info.ErrTempers.bits.Store = 1;
		error |= ERR_Lv1;						//һ������
	}
	else
	{
		sys_Info.ErrTempers.bits.Store = 0;
	}
	
	if(sys_Info.CANRecvErrTimes >= 10)			//CANͨ��ʧ��
	{
		sys_Info.ErrDevices.bits.CANFault = 1;
		error |= ERR_Lv1;						//һ������
	}
	else
	{
		sys_Info.ErrDevices.bits.CANFault = 0;
	}
	
	if(VFDRun_dFault_GetVal() == 0)				//��Ƶ������
	{
		sys_Info.ErrDevices.bits.VFDFault = 1;
		error |= ERR_Lv2;						//��������
	}
	else
	{
		sys_Info.ErrDevices.bits.VFDFault = 0;
	}

	if(CompOverPress_dFault_GetVal() == 1)		//ѹ������ѹ���ߵ�ƽ����
	{
		sys_Info.ErrDevices.bits.CompOverPres = 1;
		error |= ERR_Lv2;						//��������
	}
	else
	{
		sys_Info.ErrDevices.bits.CompOverPres = 0;
	}
	
	if(CompLowPress_dFault_GetVal() == 1)		//ѹ����Ƿѹ���ߵ�ƽ����
	{
		sys_Info.ErrDevices.bits.CompLowPres = 1;
		error |= ERR_Lv2;						//��������
	}
	else
	{
		sys_Info.ErrDevices.bits.CompLowPres = 0;
	}
	
	if(sys_Info.CompTmpErrTimes >= 20)			//ѹ�������»򴫸�����
	{
		sys_Info.ErrTempers.bits.CompHeat = 1;
		error |= ERR_Lv3;						//��������
	}
	else
	{
		sys_Info.ErrTempers.bits.CompHeat = 0;
	}
	
	if(sys_Info.BusInTmpErrTimes >= 20)			//�����¶ȴ���������
	{
		sys_Info.ErrTempers.bits.BusIn = 1;
		error |= ERR_Lv3;						//��������
	}
	else
	{
		sys_Info.ErrTempers.bits.BusIn = 0;
	}

	if(sys_Info.BusOutTmpErrTimes >= 20)		//���⡢���̹ܡ����̹��¶ȴ��������ϣ�����������������
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
		gPowerErrCheckTcnt = TIME_SEC * 60;		//�ػ����ܵ��⵹��ʱ���ܿ�DCDC�ϵ��40s���ϱ���
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
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);			//�����������
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
				sys_Ctrl.CompOnEn = 0;	//ѹ����1�����Ժ������
				Close_Valve();
			}
			if(user_Info.Mode == MODE_COLD)
			{
				sys_Ctrl.InnerFanLv = user_Info.WindLv;
				sys_Ctrl.OutsideFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);			//�����������
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

