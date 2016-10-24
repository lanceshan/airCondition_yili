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
	sys_Ctrl.InnerFanLv = 0;						//�ڻ��ر�
	sys_Ctrl.OutsideFanLv = 0;						//����ر�
	sys_Ctrl.PTCLv = 0;								//�ر�PTC
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

void AutoModeProcess(void)							//�Զ�ģʽ�����ͻش������
{
	if(sys_Info.BusInTmp > AUTO_COLD_TEMPER)		//�Զ�����ģʽ
	{
		gcur_state = state_compcold;
		user_Info.Temper = AUTO_COLD_TEMPER - 1;	//�趨�¶�Ϊ��ֵ���������û��趨�¶ȿ���
		sys_Ctrl.PTCLv = 0;
		sys_Ctrl.InnerFanLv = Tactic_AutoColdInnerFanLv();	//�Զ�ģʽ������״̬������������ж�
		sys_Ctrl.CompLv = Tactic_ColdCmpLv();		//�Զ�ģʽ������״̬ѹ����Ƶ���ж�				
		sys_Ctrl.OutsideFanLv = Tactic_ColdOutsideFanLv(sys_Ctrl.CompLv);//�Զ�ģʽ������״̬�������ת���ж�
		
		Close_Valve();	
		Set_PTC(sys_Ctrl.PTCLv);
		Set_Cmp(sys_Ctrl.CompLv);
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
	else if(sys_Info.BusInTmp < AUTO_HOT_TEMPER)		//�Զ�����ģʽ
	{
		user_Info.Temper = AUTO_HOT_TEMPER + 1;
		
		if(sys_Info.BusOutTmp < PTC_Wks_TEMPER)		//����¶ȵ���-5��ʱ��ֻ��PTC����
		{	
			gcur_state = state_ptchot;
			sys_Ctrl.PTCLv = Tactic_PTC();
			Set_PTC(sys_Ctrl.PTCLv);				
			sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//�ڻ���������
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)sys_Ctrl.CompOnEn = 0;
			sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
		}
		else										//����¶ȸ���-5���ҵ���8��ʱ����������ѹ�������ȣ�����ֶ�PTC����PTC����������PTC������
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
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//�ڻ���������
		}			
	}
	else if((sys_Info.BusInTmp > (AUTO_HOT_TEMPER + 1)) && (sys_Info.BusInTmp < (AUTO_COLD_TEMPER - 1)))	//�Զ�ͨ��ģʽ
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
	Set_Cmp(sys_Ctrl.CompLv);						//ѹ��������
	Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);		//�ڻ�����
	Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);	//�������
}

void HotModeProcess(void)
{
	if(sys_Info.BusOutTmp < PTC_Wks_TEMPER)		//����¶ȵ���-5��ʱ��ֻ��PTC����
	{	
		gcur_state = state_ptchot;
		sys_Ctrl.PTCLv = Tactic_PTC();
		Set_PTC(sys_Ctrl.PTCLv);				
		sys_Ctrl.InnerFanLv = Tactic_HotInnerFanLv(gcur_state);
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//�ڻ���������
		Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
		if(sys_Ctrl.CompLv == 0)sys_Ctrl.CompOnEn = 0;
		sys_Ctrl.OutsideFanLv = Tactic_HotOutsideFanLv(sys_Ctrl.CompLv);
		Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
	}
	else										//����¶ȸ���-5��ʱ������ѹ�������ȣ�����ֶ�PTC����PTC����������PTC������
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
		Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//�ڻ���������
	}
}

void BlowerModeProcess(void)		//����ģʽ���ر�ѹ���������������PTC
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

void HomeModeProcess(void)			//Ԥ��
{
	;
}

void TestModeProcess(void)			//�����ӱ������򣬲�������ʱ���޶����¶ȱ�����
{
	if((user_Info.TestIndex <= 0) || (user_Info.TestIndex > 3))return;
	switch(user_Info.TestIndex)
	{
		case 1:											//����ѹ��������
			Close_Valve();								//��ͨ����
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//����������ת��
			sys_Ctrl.CompLv = 8;
			Set_Cmp(sys_Ctrl.CompLv);					//ѹ����Ƶ�����
			sys_Ctrl.OutsideFanLv = 2;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);//���������Ƶ
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);					//�ر�PTC
			break;
		case 2:											//����ѹ��������
			Open_Valve();								//��ͨ����
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//����������ת��
			sys_Ctrl.CompLv = 8;
			Set_Cmp(sys_Ctrl.CompLv);					//ѹ����Ƶ�����
			sys_Ctrl.OutsideFanLv = 2;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);//���������Ƶ	
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);					//�ر�PTC
			break;
		case 3:											//����PTC����					
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			sys_Ctrl.OutsideFanLv = 0;
			Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
			sys_Ctrl.InnerFanLv = 6;
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);	//����������������
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
		sFrostStep = 5;		//��˪���������Ÿ�ֵ��׼����ʼ��˪
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
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);	//�ر�ѹ�����ͷ��
			if(sys_Ctrl.CompLv == 0)
			{
				sys_Ctrl.CompOnEn = 0;				//ѹ����1�����Ժ������
				sys_Ctrl.OutsideFanLv = 0;
				Set_OutsideFan_Speed(sys_Ctrl.OutsideFanLv);
				sys_Ctrl.InnerFanLv = 0;
				Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
				sFrostStep = 4;
			}
			break;
		case 4:
			if(sys_Ctrl.CompOffTime >= 40*TIME_SEC)	//ѹ�����رպ�40s���ر���ͨ��
			{
				Close_Valve();
				sFrostStep = 3;
			}
			break;
		case 3:
			if(sys_Ctrl.CompOffTime >= 60*TIME_SEC)	//	��ʼ��˪
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
		case 2:										//�˳���˪ģʽ
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);//�ر�ѹ�����ͷ��
			if(sys_Ctrl.CompLv == 0)
			{
				sFrostStep = 1;
				sys_Ctrl.CompOnEn = 0;				//ѹ����1�����Ժ������
			}
			break;
		case 1:
			if(sys_Ctrl.CompOffTime >= 40*TIME_SEC)	//ѹ�����رպ�40s������ͨ��
			{
				Open_Valve();
			}
			if(sys_Ctrl.CompOffTime >= 55*TIME_SEC)	//ѹ�����رպ�55s���ָ�����ģʽ
			{
				sFrostStep = 0;
			}
			break;
		default:
			break;
	}
	return sFrostStep;
}

unsigned char Power_OFFProcess(unsigned char offstep)	//�ػ������ر�ѹ���������������PTC���·�
{
	switch (offstep)
	{
		case 3:
			gOFFModeTcnt = OFF_MODE_TIME;		//�ػ�״̬18s����ʱ
			offstep = 2;
			break;
		case 2: 
			sys_Ctrl.PTCLv = 0;
			Set_PTC(sys_Ctrl.PTCLv);
			
			sys_Ctrl.InnerFanLv = 0;				//�ػ�������������ٹ���
			Set_InnerFan_Speed(sys_Ctrl.InnerFanLv);
			Close_Cmp(&sys_Ctrl.CompLv, &sys_Ctrl.CompOffStepTime);
			if(sys_Ctrl.CompLv == 0)
			{
				sys_Ctrl.CompOnEn = 0;	//ѹ����1�����Ժ������
				gOFFModeTcnt = OFF_MODE_TIME;		//�ػ�״̬18s����ʱ
				offstep = 1;
			}
			break;
		case 1:
			if(gOFFModeTcnt == 0)		//18s�󣬹ر������������Դ�ж�
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
			gPowerErrCheckTcnt = TIME_SEC * 60;	//�ػ����ܵ��⵹��ʱ���ܿ�DCDC�ϵ��40s���ϱ���
			gDCErrCheckTcnt = TIME_SEC * 20;	//������DC��⵹��ʱ
			break;
	}
	gSysStartTcnt = TIME_SYS_START;
	return offstep;
}

void Fresh_Fan_Ctrl(unsigned char new_Cmd)
{
	static unsigned char sCurPos_Left = FRESH_POS_CLOSE;
	static unsigned char sFresh_Init_OK = 0;
	if(sFresh_Init_OK  == 0)			//�·����ϵ��λ,�Զ��ر�
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
		case FRESH_FAN_OFF:						//�յ��ر������ͬ��λ������Ӧ����
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
		case FRESH_FAN_LOW:						//�յ��뿪�����ͬ��λ������Ӧ����
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
		case FRESH_FAN_HIGH:					//�յ�ȫ�������ͬ��λ������Ӧ����
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
		next_mode = MODE_PAUSE;				//����2s��������PAUSEģʽ��ϵͳһ���豸������
		return next_mode;			
	}
	
	Fresh_Fan_Ctrl(user_Info.FreshAir);
	if(sys_Info.StoreTmp >= 50)Open_StoreFan();
	if(sys_Info.StoreTmp <= 45)Close_StoreFan();
	
	if(0 != FrostModeProcess())				//�Ƚ��г�˪ģʽ�жϴ���
	{
		next_mode = MODE_PAUSE;				//�û�ģʽ��ͣ��ֻ���г�˪����
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
					gHotToColdTcnt = HOT_TO_COLD_TIME;		//���ȵ�����ʱ�ȴ�1����
			}
			if(cur_mode == MODE_COLD)
			{
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)
					gColdToHotTcnt = COLD_TO_HOT_TIME;		//���䵽����ʱ�ȴ�3����
			}	
			if((gHotToColdTcnt == 0) && (gColdToHotTcnt == 0))
				next_mode = MODE_AUTO;
			else
				next_mode = MODE_HALT;
			break;		
		case MODE_COLD:
			if(cur_mode == MODE_HOT)
				gHotToColdTcnt = HOT_TO_COLD_TIME;			//���ȵ�����ʱ�ȴ�1����
			if(cur_mode == MODE_AUTO)
			{
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)	//�Զ�ģʽ�£��жϵ�ǰģʽΪ����
					gHotToColdTcnt = HOT_TO_COLD_TIME;		//���ȵ�����1���ӵȴ�
			}
			if(gHotToColdTcnt == 0)
				next_mode = MODE_COLD;
			else
				next_mode = MODE_HALT;
			break;
		case MODE_HOT:
			if(cur_mode == MODE_COLD)
				gColdToHotTcnt = COLD_TO_HOT_TIME;			//�����л�������ʱ�ȴ�3����
			if(cur_mode == MODE_AUTO)
			{
				if(sys_Info.BusInTmp >= AUTO_COLD_TEMPER)	//�Զ�ģʽ�£��жϵ�ǰģʽΪ����
					gColdToHotTcnt = COLD_TO_HOT_TIME;		//���䵽����ʱ�ȴ�3����
			}
			if(gColdToHotTcnt == 0)
				next_mode = MODE_HOT;
			else
				next_mode = MODE_HALT;
			break;		
		case MODE_BLOW:
			if(cur_mode == MODE_HOT)
				gHotToColdTcnt = HOT_TO_COLD_TIME;		//���ȵ�����ʱ�ȴ�1���ӣ���ֹ������ģʽ�л���ͨ��ģʽ��Ȼ���������л�������ģʽ
			if(cur_mode == MODE_COLD)
				gColdToHotTcnt = COLD_TO_HOT_TIME;		//���䵽����ʱ�ȴ�3���ӣ���ֹ������ģʽ�л���ͨ��ģʽ��Ȼ���������л�������ģʽ
			if(cur_mode == MODE_AUTO)					//���Զ�ģʽ�л���ͨ��ģʽ���������л������Ȼ�����
			{
				if(sys_Info.BusInTmp >= AUTO_COLD_TEMPER)
					gColdToHotTcnt = COLD_TO_HOT_TIME;
				if(sys_Info.BusInTmp <= AUTO_HOT_TEMPER)
					gHotToColdTcnt = HOT_TO_COLD_TIME;
			}
			next_mode = MODE_BLOW;
			break;
		case MODE_HOME:
			next_mode = MODE_HOME;		//Ԥ��
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
