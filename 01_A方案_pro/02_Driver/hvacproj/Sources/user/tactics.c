/*
 * tactics.c
 *
 *  Created on: Aug 16, 2015
 *      Author: Hill
 */
#include "tactics.h"
#include "globalvars.h"

static float pre_difftemper = -20;		//上次的温差
static float now_difftemper = 0; 		//当前的温差
static float pre_outtemper = 0;			//上次的车外温度
static unsigned char sDifftmp_trend= 0;
static unsigned char sOuttmp_trend= 0;	
static unsigned char sOutsideFanLv = 0;	//冷凝风机频率
void Tactic_TmpTrend(void)				//温差变化趋势判断，室外温度变化趋势判断，每5s判断一次
{
	if(gTrendChgTcnt == 0)
	{
		now_difftemper = user_Info.Temper - (sys_Info.BusInTmp + 0.1*((unsigned int)(sys_Info.BusOutTmp * 10)/20));	
		if((now_difftemper - pre_difftemper) >= 0.2)
			sDifftmp_trend = 1;
		else
			sDifftmp_trend = 0;
		
		if((sys_Info.BusOutTmp - pre_outtemper) >= 0.2)
			sOuttmp_trend = 1;
		else
			sOuttmp_trend = 0;
		
		pre_outtemper = sys_Info.BusOutTmp;
		pre_difftemper = now_difftemper;
		gTrendChgTcnt = TREND_CHANGE_TIME;
	}	
}

unsigned char Tactic_AutoColdInnerFanLv(void)		//自动模式下制冷时内机风量温控策略
{
	unsigned char windLv = 1;				//风量共9级，初始为1级
	
	Tactic_TmpTrend();						//温度变化趋势判断
		
	if(sDifftmp_trend == 1)					//制冷模式下温差上升时，压缩机频率温控策略，滞回处理，防抖动
	{
		if(now_difftemper > -2.5)
			windLv = 1;
		else if((now_difftemper <= -2.5) && (now_difftemper > -3.2))
			windLv = 2;
		else if((now_difftemper <= -3.2) && (now_difftemper > -3.8))
			windLv = 3;
		else if((now_difftemper <= -3.8) && (now_difftemper > -4.4))
			windLv = 4;
		else if((now_difftemper <= -4.4) && (now_difftemper > -5.0))
			windLv = 4;
		else if((now_difftemper <= -5.0) && (now_difftemper > -5.5))
			windLv = 5;
		else if((now_difftemper <= -5.5) && (now_difftemper > -6.2))
			windLv = 5;
		else if((now_difftemper <= -6.2) && (now_difftemper > -6.5))
			windLv = 6;
		else if(now_difftemper <= -6.5)
			windLv = 6;
	}
	else
	{
		if(now_difftemper > -2.0)
			windLv = 1;
		else if((now_difftemper <= -2.0) && (now_difftemper > -2.5))
			windLv = 2;
		else if((now_difftemper <= -2.5) && (now_difftemper > -3.2))
			windLv = 3;
		else if((now_difftemper <= -3.2) && (now_difftemper > -3.8))
			windLv = 4;
		else if((now_difftemper <= -3.8) && (now_difftemper > -4.4))
			windLv = 4;
		else if((now_difftemper <= -4.4) && (now_difftemper > -5.0))
			windLv = 5;
		else if((now_difftemper <= -5.0) && (now_difftemper > -5.5))
			windLv = 5;
		else if((now_difftemper <= -5.5) && (now_difftemper > -6.2))
			windLv = 6;
		else if(now_difftemper <= -6.2)
			windLv = 6;
	}
	return windLv;
}

unsigned char Tactic_AutoHotInnerFanLv(void)		//自动模式下制热时内机风量温控策略
{
	unsigned char windLv = 1;
	unsigned char windLv_max = 0;
	Tactic_TmpTrend();
	
	if(sDifftmp_trend == 1)
	{
		if(now_difftemper < -0.3)
			windLv = 0;
		else if((now_difftemper >= -0.3) && (now_difftemper < 0.4))
			windLv = 1;
		else if((now_difftemper >= 0.4) && (now_difftemper < 1.1))
			windLv = 2;
		else if((now_difftemper >= 1.1) && (now_difftemper < 1.8))
			windLv = 3;
		else if((now_difftemper >= 1.8) && (now_difftemper < 2.5))
			windLv = 4;
		else if((now_difftemper >= 2.5) && (now_difftemper < 3.2))
			windLv = 4;
		else if((now_difftemper >= 3.2) && (now_difftemper < 3.9))
			windLv = 5;
		else if((now_difftemper >= 3.9) && (now_difftemper < 4.6))
			windLv = 5;
		else if((now_difftemper >= 4.6) && (now_difftemper < 5.3))
			windLv = 6;
		else if(now_difftemper >= 5.3)
			windLv = 6;
	}
	else
	{
		if(now_difftemper < -1)
			windLv = 0;
		else if((now_difftemper >= -1) && (now_difftemper < -0.3))
			windLv = 1;
		else if((now_difftemper >= -0.3) && (now_difftemper < 0.4))
			windLv = 2;
		else if((now_difftemper >= 0.4) && (now_difftemper < 1.1))
			windLv = 3;
		else if((now_difftemper >= 1.1) && (now_difftemper < 1.8))
			windLv = 4;
		else if((now_difftemper >= 1.8) && (now_difftemper < 2.5))
			windLv = 4;
		else if((now_difftemper >= 2.5) && (now_difftemper < 3.2))
			windLv = 5;
		else if((now_difftemper >= 3.2) && (now_difftemper < 3.9))
			windLv = 5;
		else if((now_difftemper >= 3.9) && (now_difftemper < 4.6))
			windLv = 6;
		else if(now_difftemper >= 4.6)
			windLv = 6;
	}
	
	if(sys_Info.InPipeTmp < 30)
	{
		windLv_max = 0;
	}
	else
	{
		if(sys_Info.InPipeTmp < 25)
			windLv_max = 1;
		else if(sys_Info.InPipeTmp < 35)
			windLv_max = 2;
		else if(sys_Info.InPipeTmp < 40)
			windLv_max = 3;
		else if(sys_Info.InPipeTmp < 50)
			windLv_max = 4;
		else
			windLv_max = 5;		
	}
	if(windLv > windLv_max)windLv = windLv_max;
	
	return windLv;
}

unsigned char Tactic_ColdCmpLv(void)		//制冷模式下压缩机频率控制策略
{
	unsigned char cmpLv = 0;				//压缩机档位共8级
	unsigned char cmpLv_max = 2;			//最大工作频率
	
	if(sys_Ctrl.CompOnEn == 0)return cmpLv;
	Tactic_TmpTrend();						//温度变化趋势判断
	
	if(sDifftmp_trend == 1)					//制冷模式下温差上升时，压缩机频率温控策略，滞回处理，防抖动
	{
		if(now_difftemper >= -2.5)
			cmpLv = 0;
		else if((now_difftemper >= -3.8) && (now_difftemper < -2.5))
			cmpLv = 1;
		else if((now_difftemper >= -4.3) && (now_difftemper < -3.8))
			cmpLv = 2;
		else if((now_difftemper >= -4.8) && (now_difftemper < -4.3))
			cmpLv = 3;
		else if((now_difftemper >= -5.3) && (now_difftemper < -4.8))
			cmpLv = 4;
		else if((now_difftemper >= -5.8) && (now_difftemper < -5.3))
			cmpLv = 5;
		else if((now_difftemper >= -6.3) && (now_difftemper < -5.8))
			cmpLv = 6;
		else if((now_difftemper >= -6.8) && (now_difftemper < -6.3))
			cmpLv = 7;
		else if(now_difftemper < -6.8)
			cmpLv = 8;
	}
	else		//制冷模式下温差下降时，压缩机频率温控策略，滞回处理，防抖动
	{
		if(now_difftemper >= 1.0)
			cmpLv = 0;
		else if((now_difftemper >= -2.7) && (now_difftemper < 1.0))
			cmpLv = 1;
		else if((now_difftemper >= -3.2) && (now_difftemper < -2.7))
			cmpLv = 2;
		else if((now_difftemper >= -3.7) && (now_difftemper < -3.2))
			cmpLv = 3;
		else if((now_difftemper >= -4.2) && (now_difftemper < -3.7))
			cmpLv = 4;
		else if((now_difftemper >= -4.7) && (now_difftemper < -4.2))
			cmpLv = 5;
		else if((now_difftemper >= -5.2) && (now_difftemper < -4.7))
			cmpLv = 6;
		else if((now_difftemper >= -5.7) && (now_difftemper < -5.2))
			cmpLv = 7;
		else if(now_difftemper < -5.7)
			cmpLv = 8;
	}	
	
	if(sOuttmp_trend == 1)
	{
		if(sys_Info.BusOutTmp < 22)		//根据车外温度确定压缩机工作最高频率
			cmpLv_max = 2;
		else if((sys_Info.BusOutTmp >= 22) && (sys_Info.BusOutTmp < 26))
			cmpLv_max = 4;
		else if((sys_Info.BusOutTmp >= 26) && (sys_Info.BusOutTmp < 30))
			cmpLv_max = 6;
		else if(sys_Info.BusOutTmp >= 30)
			cmpLv_max = 8;
	}
	else
	{
		if(sys_Info.BusOutTmp < 20)		//根据车外温度确定压缩机工作最高频率
			cmpLv_max = 2;
		else if((sys_Info.BusOutTmp >= 20) && (sys_Info.BusOutTmp < 24))
			cmpLv_max = 4;
		else if((sys_Info.BusOutTmp >= 24) && (sys_Info.BusOutTmp < 28))
			cmpLv_max = 6;
		else if(sys_Info.BusOutTmp >= 28)
			cmpLv_max = 8;
	}
	if(cmpLv >= cmpLv_max)
		cmpLv = cmpLv_max;				//压缩机最高频率限制
	if((sys_Ctrl.CompHiLvEn == 0) && (cmpLv > 5))cmpLv = 5;
	if(cmpLv == 0)sys_Ctrl.CompOnEn = 0;//压缩机1分钟以后允许打开
	return cmpLv;
}

unsigned char Tactic_HotCmpLv(void)		//制热模式下压缩机频率控制策略
{
	unsigned char cmpLv = 0;
	unsigned char cmpLv_max = 0;		//最大工作频率
	if(sys_Ctrl.CompOnEn == 0)return cmpLv;
	Tactic_TmpTrend();					//温度变化趋势判断
	if(sDifftmp_trend == 1)				//制热模式下温控策略，滞回处理，防抖动
	{
		if(now_difftemper <= 0.4)
			cmpLv = 0;
		else if((now_difftemper >= 0.4) && (now_difftemper < 2.2))
			cmpLv = 1;
		else if((now_difftemper >= 2.2) && (now_difftemper < 2.5))
			cmpLv = 2;
		else if((now_difftemper >= 2.5) && (now_difftemper < 2.8))
			cmpLv = 3;
		else if((now_difftemper >= 2.8) && (now_difftemper < 3.1))
			cmpLv = 4;
		else if((now_difftemper >= 3.1) && (now_difftemper < 3.4))
			cmpLv = 5;
		else if((now_difftemper >= 3.4) && (now_difftemper < 3.7))
			cmpLv = 6;
		else if((now_difftemper >= 3.7) && (now_difftemper < 4.0))
			cmpLv = 7;
		else if(now_difftemper >= 4.0)
			cmpLv = 8;
	}
	else
	{
		if(now_difftemper <= -3)
			cmpLv = 0;
		else if((now_difftemper >= -3) && (now_difftemper < 0.9))
			cmpLv = 1;
		else if((now_difftemper >= 0.9) && (now_difftemper < 1.2))
			cmpLv = 2;
		else if((now_difftemper >= 1.2) && (now_difftemper < 1.5))
			cmpLv = 3;
		else if((now_difftemper >= 1.5) && (now_difftemper < 1.8))
			cmpLv = 4;
		else if((now_difftemper >= 1.8) && (now_difftemper < 2.1))
			cmpLv = 5;
		else if((now_difftemper >= 2.1) && (now_difftemper < 2.4))
			cmpLv = 6;
		else if((now_difftemper >= 2.4) && (now_difftemper < 2.7))
			cmpLv = 7;
		else if(now_difftemper >= 2.7)
			cmpLv = 8;
	}	
	
	if(sOuttmp_trend == 1)
	{
		if(sys_Info.BusOutTmp > 19)		//根据车外温度确定压缩机工作最高频率
			cmpLv_max = 2;
		else if((sys_Info.BusOutTmp > 15) && (sys_Info.BusOutTmp <= 19))
			cmpLv_max = 4;
		else if((sys_Info.BusOutTmp > 11) && (sys_Info.BusOutTmp <= 15))
			cmpLv_max = 6;
		else if(sys_Info.BusOutTmp <= 11)
			cmpLv_max = 8;
	}
	else
	{
		if(sys_Info.BusOutTmp > 17)		//根据车外温度确定压缩机工作最高频率
			cmpLv_max = 2;
		else if((sys_Info.BusOutTmp > 13) && (sys_Info.BusOutTmp <= 17))
			cmpLv_max = 4;
		else if((sys_Info.BusOutTmp > 9) && (sys_Info.BusOutTmp <= 13))
			cmpLv_max = 6;
		else if(sys_Info.BusOutTmp <= 9)
			cmpLv_max = 8;
	}
	if(cmpLv >= cmpLv_max)cmpLv = cmpLv_max;//压缩机最高频率限制
	if((sys_Ctrl.CompHiLvEn == 0) && (cmpLv > 5))cmpLv = 5;
	if(cmpLv == 0)sys_Ctrl.CompOnEn = 0;	//压缩机1分钟以后允许打开
	return cmpLv;
}

unsigned char Tactic_HotInnerFanLv(unsigned char hot_mode)
{
	unsigned char fanlv = 0;
	if((sys_Info.InPipeTmp < 30) && (hot_mode == state_comphot))
	{
		fanlv = 0;
	}
	else
	{
		if((sys_Info.InPipeTmp > -50) && (sys_Info.InPipeTmp <= 25))
			fanlv = 1;
		else if((sys_Info.InPipeTmp > 25) && (sys_Info.InPipeTmp <= 35))
			fanlv = 2;
		else if((sys_Info.InPipeTmp > 35) && (sys_Info.InPipeTmp <= 45))
			fanlv = 3;
		else if((sys_Info.InPipeTmp > 45) && (sys_Info.InPipeTmp <= 50))
			fanlv = 4;
		else
			fanlv = 5;	
	}
	return fanlv;
}

unsigned char Tactic_ColdOutsideFanLv(unsigned char cmpLv)	//制冷模式下冷凝风机的工作频率
{	
	if(cmpLv == 0)
	{
		if(sys_Ctrl.CompOffTime > (70*TIME_SEC))
			sOutsideFanLv = 0;
		return sOutsideFanLv;
	}

	if(sys_Info.BusOutTmp >= 30)
		sOutsideFanLv = 2;
	if(sys_Info.BusOutTmp <= 28)
		sOutsideFanLv = 1;
	
	if(cmpLv >= 5)
		sOutsideFanLv = 2;
	if(cmpLv <= 3)											//压缩机工作频率判断冷凝风机工作频率，压机频率高于62时，高频率工作，低于42时，低频率工作
		sOutsideFanLv = 1;
	
	return sOutsideFanLv;
}

unsigned char Tactic_HotOutsideFanLv(unsigned char cmpLv)	//制热模式下冷凝风机的工作频率
{
	if(cmpLv == 0)
	{
		if(sys_Ctrl.CompOffTime > (70*TIME_SEC))
			sOutsideFanLv = 0;
		return sOutsideFanLv;
	}
	if(cmpLv >= 4)
		sOutsideFanLv = 2;
	if(cmpLv <= 3)
		sOutsideFanLv = 1;

	if(sys_Info.BusOutTmp >= 10)
		sOutsideFanLv = 1;
	if(sys_Info.BusOutTmp <= 7)
		sOutsideFanLv = 2;
	
	return sOutsideFanLv;
}

unsigned char Tactic_PTC(void)			//PTC控制策略
{
	static unsigned char ptcnum = 0;	//ptc 工作组数
	
	Tactic_TmpTrend();					//温度变化趋势判断

	if(now_difftemper >= 0)
		ptcnum = 1;
	if(now_difftemper <= -0.3)
		ptcnum = 0;

	return ptcnum;
}
