/*
 * mode.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "mode.h"
#include "tactics.h"
#include "globalvars.h"
#include "function\power.h"
#include "function\fan.h"
#include "function\compressor.h"
#include "function\ptc.h"
#include "function\valve.h"
static const tModeProcess ModeProcessArr[] = {HaltModeProcess, ColdModeProcess, HotModeProcess, BlowerModeProcess, AutoModeProcess, HomeModeProcess, TestModeProcess, PauseModeProcess};

void HaltModeProcess(void)
{
	gcur_state = state_halt;
	sys_Ctrl.InnerFanLv = 0;						//内机关闭
	sys_Ctrl.OutsideFanLv = 0;						//外机关闭
	sys_Ctrl.PTCLv = 0;								//关闭PTC
	Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
	if(sys_Ctrl.CompOffTime > (70*TIME_SEC))
	{
		Close_Valve();
		sys_Ctrl.OutsideFanLv = 0;
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
	Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);			
	Set_PTC(sys_Ctrl.PTCLv);
}

void AutoModeProcess(void)							//自动模式处理，滞回处理防抖
{
	if(sys_Info.BusInTmp > AUTO_COLD_TEMPER)		//自动制冷模式
	{
		gcur_state = state_compcold;
		user_Info.Temper = AUTO_COLD_TEMPER - 1;	//设定温度为定值，不再受用户设定温度控制
		sys_Ctrl.PTCLv = 0;
		sys_Ctrl.InnerFanLv = Tactic_AutoColdInnerFanLv();	//自动模式下制冷状态蒸发风机风量判断
		sys_Ctrl.CompLv = Tactic_ColdCmpLv();		//自动模式下制冷状态压缩机频率判断				
		sys_Ctrl.OutsideFanLv = Tactic_ColdOutsideFanLv(sys_Ctrl.CompLv);//自动模式下制冷状态冷凝风机转速判断
		
		Close_Valve();	
		Set_PTC(sys_Ctrl.PTCLv);
		Set_Cmp(sys_Ctrl.CompLv);
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
	else if(sys_Info.BusInTmp < AUTO_HOT_TEMPER)		//自动制热模式
	{
		user_Info.Temper = AUTO_HOT_TEMPER + 1;
		
		if(sys_Info.BusOutTmp < PTC_Wks_TEMPER)		//外界温度低于-5℃时，只有PTC制热
		{	
			gcur_state = state_ptchot;
			sys_Ctrl.PTCLv = Tactic_PTC();
			Set_PTC(sys_Ctrl.PTCLv);				
			sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//内机风量设置
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)sys_Ctrl.CompOnEn = 0;
			sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
		}
		else										//外界温度高于-5℃且低于8℃时，可以启动压缩机制热，如果手动PTC打开则PTC工作，否则PTC不工作
		{	
			if(sys_Info.BusOutTmp > PTC_STOP_TEMPER)
			{
				gcur_state = state_comphot;
				sys_Ctrl.PTCLv = 0;
				Set_PTC(sys_Ctrl.PTCLv);
			}
			else
			{
				if(user_Info.PTC == 1)
				{
					gcur_state = state_bothhot;
					sys_Ctrl.PTCLv = Tactic_PTC();
					Set_PTC(sys_Ctrl.PTCLv);			
				}
				else
				{
					gcur_state = state_comphot;
					sys_Ctrl.PTCLv = 0;
					Set_PTC(sys_Ctrl.PTCLv);			
				}
			}
			Open_Valve();						
			sys_Ctrl.CompLv = Tactic_HotCmpLv();		
			Set_Cmp(sys_Ctrl.CompLv);
			sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//内机风量设置
		}			
	}
	else if((sys_Info.BusInTmp > (AUTO_HOT_TEMPER + 1)) && (sys_Info.BusInTmp < (AUTO_COLD_TEMPER - 1)))	//自动通风模式
	{
		gcur_state = state_blow;
		sys_Ctrl.InnerFanLv = Tactic_AutoColdInnerFanLv();
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
		sys_Ctrl.PTCLv = 0;
		Set_PTC(sys_Ctrl.PTCLv);
		Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
		if(sys_Ctrl.CompOffTime > (70*TIME_SEC))
		{
			Close_Valve();
			sys_Ctrl.OutsideFanLv = 0;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
		}
	}	
}

void ColdModeProcess(void)
{
	gcur_state = state_compcold;

	sys_Ctrl.InnerFanLv = user_Info.WindLv;
	sys_Ctrl.CompLv = Tactic_ColdCmpLv();			
	sys_Ctrl.OutsideFanLv = Tactic_ColdOutsideFanLv(sys_Ctrl.CompLv);

	Close_Valve();
	Set_Cmp(sys_Ctrl.CompLv);						//压缩机设置
	Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);		//内机设置
	Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);	//外机设置
}

void HotModeProcess(void)
{
	if(sys_Info.BusOutTmp < PTC_Wks_TEMPER)		//外界温度低于-5℃时，只有PTC制热
	{	
		gcur_state = state_ptchot;
		sys_Ctrl.PTCLv = Tactic_PTC();
		Set_PTC(sys_Ctrl.PTCLv);				
		sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//内机风量设置
		Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
		if(sys_Ctrl.CompLv == 0)sys_Ctrl.CompOnEn = 0;
		sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
	else										//外界温度高于-5℃时，启动压缩机制热，如果手动PTC打开则PTC工作，否则PTC不工作
	{	
		if(sys_Info.BusOutTmp > PTC_STOP_TEMPER)
		{
			gcur_state = state_comphot;
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);
		}
		else
		{
			if(user_Info.PTC == 1)
			{
				gcur_state = state_bothhot;
				sys_Ctrl.PTCLv = Tactic_PTC();
				Set_PTC(sys_Ctrl.PTCLv);			
			}
			else
			{
				gcur_state = state_comphot;
				sys_Ctrl.PTCLv = 0;
				Set_PTC(sys_Ctrl.PTCLv);			
			}
		}
		Open_Valve();						
		sys_Ctrl.CompLv = Tactic_HotCmpLv();		
		Set_Cmp(sys_Ctrl.CompLv);
		sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
		sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//内机风量设置
	}
}

void BlowerModeProcess(void)		//吹风模式，关闭压缩机、冷凝风机、PTC
{
	gcur_state = state_blow;
	
	sys_Ctrl.InnerFanLv = user_Info.WindLv;
	Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
	sys_Ctrl.PTCLv = 0;
	Set_PTC(sys_Ctrl.PTCLv);
	Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
	if(sys_Ctrl.CompOffTime > (70*TIME_SEC))
	{
		Close_Valve();
		sys_Ctrl.OutsideFanLv = 0;
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
}

void HomeModeProcess(void)			//预留
{
	;
}

void TestModeProcess(void)			//待增加保护程序，测试运行时间限定？温度保护？
{
	if((user_Info.TestIndex <= 0) || (user_Info.TestIndex > 3))return;
	switch(user_Info.TestIndex)
	{
		case 1:											//测试压缩机制冷
			Close_Valve();								//四通阀关
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//蒸发风机最大转速
			sys_Ctrl.CompLv = 8;
			Set_Cmp(sys_Ctrl.CompLv);					//压缩机频率最大
			sys_Ctrl.OutsideFanLv = 2;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);//冷凝风机高频
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);					//关闭PTC
			break;
		case 2:											//测试压缩机制热
			Open_Valve();								//四通阀开
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//蒸发风机最大转速
			sys_Ctrl.CompLv = 8;
			Set_Cmp(sys_Ctrl.CompLv);					//压缩机频率最大
			sys_Ctrl.OutsideFanLv = 2;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);//冷凝风机高频	
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);					//关闭PTC
			break;
		case 3:											//测试PTC制热					
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			sys_Ctrl.OutsideFanLv = 0;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//蒸发风机最高速运行
			sys_Ctrl.PTCLv = 2;
			Set_PTC(sys_Ctrl.PTCLv);
			break;
		default:
			break;
	}	
}

void PauseModeProcess(void)
{
	;
}

unsigned char FrostModeProcess(void)
{
	static unsigned char sFrostStep = 0;
	
	if(((gcur_state == state_comphot) || (gcur_state == state_bothhot)) 
		&& (gFrostLowTempTcnt > (30*TIME_SEC))
		&& (sys_Ctrl.CompHotTime > (30*TIME_MIN)))
	{
		gcur_state = state_frost;
		sFrostStep = 5;		//除霜步骤索引号赋值，准备开始除霜
	}
	if(((gcur_state == state_comphot) || (gcur_state == state_ptchot) || (gcur_state == state_bothhot))
		&& (sys_Info.BusOutTmp <= 3) 
		&& (sys_Ctrl.CompHotTime > (90*TIME_MIN)))
	{
		gcur_state = state_frost;
		sFrostStep = 5;
	}
	
	switch(sFrostStep)
	{
		case 5:
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);	//关闭压缩机和风机
			if(sys_Ctrl.CompLv == 0)
			{
				sys_Ctrl.CompOnEn = 0;				//压缩机1分钟以后允许打开
				sys_Ctrl.OutsideFanLv = 0;
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
				sys_Ctrl.InnerFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
				sFrostStep = 4;
			}
			break;
		case 4:
			if(sys_Ctrl.CompOffTime >= 40*TIME_SEC)	//压缩机关闭后40s，关闭四通阀
			{
				Close_Valve();
				sFrostStep = 3;
			}
			break;
		case 3:
			if(sys_Ctrl.CompOffTime >= 60*TIME_SEC)	//	开始除霜
			{
				sys_Ctrl.CompLv = 5;
				Set_Cmp(sys_Ctrl.CompLv);
				sys_Ctrl.OutsideFanLv = 0;
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
				sys_Ctrl.InnerFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
				Close_Valve();
				if((sys_Info.OutPipeTmp >= 30) || (gDefrostTcnt > (5*TIME_MIN)))
				{
					sFrostStep = 2;
				}
			}
			break;
		case 2:										//退出除霜模式
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);//关闭压缩机和风机
			if(sys_Ctrl.CompLv == 0)
			{
				sFrostStep = 1;
				sys_Ctrl.CompOnEn = 0;				//压缩机1分钟以后允许打开
			}
			break;
		case 1:
			if(sys_Ctrl.CompOffTime >= 40*TIME_SEC)	//压缩机关闭后40s，打开四通阀
			{
				Open_Valve();
			}
			if(sys_Ctrl.CompOffTime >= 55*TIME_SEC)	//压缩机关闭后55s，恢复正常模式
			{
				sFrostStep = 0;
			}
			break;
		default:
			break;
	}
	return sFrostStep;
}

unsigned char Power_OFFProcess(unsigned char offstep)	//关机处理，关闭压缩机、冷凝风机、PTC、新风
{
	switch (offstep)
	{
		case 3:
			gOFFModeTcnt = OFF_MODE_TIME;		//关机状态18s倒计时
			offstep = 2;
			break;
		case 2: 
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);
			
			sys_Ctrl.InnerFanLv = 0;				//关机后，蒸发风机低速工作
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)
			{
				sys_Ctrl.CompOnEn = 0;	//压缩机1分钟以后允许打开
				gOFFModeTcnt = OFF_MODE_TIME;		//关机状态18s倒计时
				offstep = 1;
			}
			break;
		case 1:
			if(gOFFModeTcnt == 0)		//18s后，关闭冷凝风机，电源切断
			{
				sys_Ctrl.OutsideFanLv = 0;
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
				Close_StoreFan();
				user_Info.Power = 0;
				Power_OFF();
				offstep = 0;
			}
			break;
		default:
			gPowerErrCheckTcnt = TIME_SEC * 60;	//关机后总电检测倒计时，避开DCDC断电后40s故障报错
			gDCErrCheckTcnt = TIME_SEC * 20;	//开机后DC检测倒计时
			break;
	}
	gSysStartTcnt = TIME_SYS_START;
	return offstep;
}

void Fresh_Fan_Ctrl(unsigned char new_Cmd)
{
	static unsigned char sCurPos_Left = FRESH_POS_CLOSE;
	static unsigned char sFresh_Init_OK = 0;
	if(sFresh_Init_OK  == 0)			//新风门上电后复位,自动关闭
	{
		if(LeftFresh_Photo_GetVal() == 0)
		{
			sFresh_Init_OK  = 1;
			gLeftFreshWks_Tcnt = 0;
			sCurPos_Left = FRESH_POS_CLOSE;
			LEFT_FRESH_DISABLE;
		}
		else
		{
			LEFT_FRESH_ENABLE;
			LEFT_FRESH_MOVE_UP;
			gLeftFreshWks_Tcnt = 2 * FRESH_FAN_WK_TIME;
		}
		return;
	}
	
	switch (new_Cmd)
	{
		case FRESH_FAN_OFF:						//收到关闭命令，不同的位置做相应处理
			if(sCurPos_Left == FRESH_POS_MIDDLE)	
			{
				LEFT_FRESH_ENABLE;
				LEFT_FRESH_MOVE_UP;
				gLeftFreshWks_Tcnt = FRESH_FAN_WK_TIME;
			}
			if(sCurPos_Left == FRESH_POS_OPEN)
			{
				LEFT_FRESH_ENABLE;
				LEFT_FRESH_MOVE_UP;
				gLeftFreshWks_Tcnt = 2 * FRESH_FAN_WK_TIME;
			}
			if(LeftFresh_Photo_GetVal() == 0)
			{
				LEFT_FRESH_DISABLE;
				gLeftFreshWks_Tcnt = 0;
				sCurPos_Left = FRESH_POS_CLOSE;
			}
			break;
		case FRESH_FAN_LOW:						//收到半开命令，不同的位置做相应处理
			if(sCurPos_Left == FRESH_POS_CLOSE)
			{
				LEFT_FRESH_MOVE_DOWN;
				LEFT_FRESH_ENABLE;
				gLeftFreshWks_Tcnt = FRESH_FAN_WK_TIME;
			}
			if(sCurPos_Left == FRESH_POS_OPEN)
			{
				LEFT_FRESH_ENABLE;
				LEFT_FRESH_MOVE_UP;
				gLeftFreshWks_Tcnt = FRESH_FAN_WK_TIME;
			}
			if(gLeftFreshWks_Tcnt == 0)
			{
				LEFT_FRESH_DISABLE;
				sCurPos_Left = FRESH_POS_MIDDLE;
			}
			break;
		case FRESH_FAN_HIGH:					//收到全开命令，不同的位置做相应处理
			if(sCurPos_Left == FRESH_POS_CLOSE)
			{
				LEFT_FRESH_ENABLE;
				LEFT_FRESH_MOVE_DOWN;
				gLeftFreshWks_Tcnt = 2 * FRESH_FAN_WK_TIME;
			}
			if(sCurPos_Left == FRESH_POS_MIDDLE)
			{
				LEFT_FRESH_ENABLE;
				LEFT_FRESH_MOVE_DOWN;
				gLeftFreshWks_Tcnt = FRESH_FAN_WK_TIME;
			}
			if(gLeftFreshWks_Tcnt == 0)
			{
				LEFT_FRESH_DISABLE;
				sCurPos_Left = FRESH_POS_OPEN;
			}
			break;
		default:
			break;
	}
	
	if(gLeftFreshWks_Tcnt != 0)
	{
		sCurPos_Left = FRESH_POS_MOVE;
	}
}

DRV_MODE Mode_Switch(DRV_MODE cur_mode)
{
	DRV_MODE next_mode = MODE_HALT;
	if(gSysStartTcnt != 0)
	{
		next_mode = MODE_PAUSE;				//开机2s以内运行PAUSE模式，系统一切设备不工作
		return next_mode;			
	}
	
	Fresh_Fan_Ctrl(user_Info.FreshAir);
	if(sys_Info.StoreTmp >= 50)Open_StoreFan();
	if(sys_Info.StoreTmp <= 45)Close_StoreFan();
	
	if(0 != FrostModeProcess())				//先进行除霜模式判断处理
	{
		next_mode = MODE_PAUSE;				//用户模式暂停，只进行除霜处理
		return next_mode;					
	}
	
	switch (user_Info.Mode)
	{
		case MODE_HALT:
			next_mode = MODE_HALT;
			break;
		case MODE_AUTO:
			if(cur_mode == MODE_HOT)
			{
				if(sys_Info.BusInTmp >= AUTO_COLD_TEMPER)
					gHotToColdTcnt = HOT_TO_COLD_TIME;		//制热到制冷时等待1分钟
			}
			if(cur_mode == MODE_COLD)
			{
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)
					gColdToHotTcnt = COLD_TO_HOT_TIME;		//制冷到制热时等待3分钟
			}	
			if((gHotToColdTcnt == 0) && (gColdToHotTcnt == 0))
				next_mode = MODE_AUTO;
			else
				next_mode = MODE_HALT;
			break;		
		case MODE_COLD:
			if(cur_mode == MODE_HOT)
				gHotToColdTcnt = HOT_TO_COLD_TIME;			//制热到制冷时等待1分钟
			if(cur_mode == MODE_AUTO)
			{
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)	//自动模式下，判断当前模式为制热
					gHotToColdTcnt = HOT_TO_COLD_TIME;		//制热到制冷1分钟等待
			}
			if(gHotToColdTcnt == 0)
				next_mode = MODE_COLD;
			else
				next_mode = MODE_HALT;
			break;
		case MODE_HOT:
			if(cur_mode == MODE_COLD)
				gColdToHotTcnt = COLD_TO_HOT_TIME;			//制冷切换到制热时等待3分钟
			if(cur_mode == MODE_AUTO)
			{
				if(sys_Info.BusInTmp >= AUTO_COLD_TEMPER)	//自动模式下，判断当前模式为制冷
					gColdToHotTcnt = COLD_TO_HOT_TIME;		//制冷到制热时等待3分钟
			}
			if(gColdToHotTcnt == 0)
				next_mode = MODE_HOT;
			else
				next_mode = MODE_HALT;
			break;		
		case MODE_BLOW:
			if(cur_mode == MODE_HOT)
				gHotToColdTcnt = HOT_TO_COLD_TIME;		//制热到制冷时等待1分钟，防止由制热模式切换到通风模式，然后立刻又切换到制冷模式
			if(cur_mode == MODE_COLD)
				gColdToHotTcnt = COLD_TO_HOT_TIME;		//制冷到制热时等待3分钟，防止由制冷模式切换到通风模式，然后立刻又切换到制热模式
			if(cur_mode == MODE_AUTO)					//由自动模式切换到通风模式，立刻又切换到制热或制冷
			{
				if(sys_Info.BusInTmp >= AUTO_COLD_TEMPER)
					gColdToHotTcnt = COLD_TO_HOT_TIME;
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)
					gHotToColdTcnt = HOT_TO_COLD_TIME;
			}
			next_mode = MODE_BLOW;
			break;
		case MODE_HOME:
			next_mode = MODE_HOME;		//预留
			break;	
		case MODE_TEST:
			next_mode = MODE_TEST;
			break;
		default:
			break;
	}
	
	return next_mode;
}

void ModeProcess(DRV_MODE cur_mode)
{
	ModeProcessArr[cur_mode]();
	
}
