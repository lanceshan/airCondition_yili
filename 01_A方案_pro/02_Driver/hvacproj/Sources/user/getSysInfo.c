/*
 * getSysInfo.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#include "getSysInfo.h"
#include "driver\adc.h"
#include "globalvars.h"
#include <math.h>
#include <cstdlib>

#define E				2.718281828459	//自然数e
#define IN_AIR_R 		5000+10
#define IN_AIR_B		3270
#define IN_AIR_T		25
#define OUT_AIR_R		1700
#define OUT_AIR_B		3900
#define OUT_AIR_T		25
#define IN_PIPE_R		4852
#define IN_PIPE_B		3820
#define IN_PIPE_T		0
#define OUT_PIPE_R		5800
#define OUT_PIPE_B		3310
#define OUT_PIPE_T		0
#define STORE_R			10000
#define STORE_B			3435
#define STORE_T			25
#define CMP_R			10000
#define CMP_B			3435
#define CMP_T			25
#define TEST_R			10000
#define TEST_B			3435
#define TEST_T			25

static float AirTemper_Buf[10] = {0};
static float OutTemper_Buf[10] = {0};
static float OutPipeTemper_Buf[10] = {0};
static float InPipeTemper_Buf[10] = {0};
static float CmpTemper_Buf[10] = {0};
static float StoreTemper_Buf[10] = {0};

static unsigned char sTimes = 0;

void SysInfo_Init(void)
{
	sys_Info.BusInTmp    = Get_AirTemper();
	sys_Info.BusOutTmp   = Get_OutTemper();		//车外环境温度
	sys_Info.InPipeTmp   = Get_InPipeTemper();
	sys_Info.OutPipeTmp  = Get_OutPipeTemper();
	sys_Info.CompTmp     = Get_CmpTemper();
	sys_Info.StoreTmp    = Get_StoreTemper();
	sys_Info.MCUTmp      = Get_MCUTemper();
}

float Average_CALC(float *buf)
{
	unsigned char i = 0;
	float temp = 0;
	for(i=0; i<10; i++)
		temp += buf[i];
	temp = (temp/sTimes) - TEMP_OFFSET;
	return temp;
}

void Get_SysInfo(void)
{
	float new_temper = 0.0;
	if(gGetSysInfoTcnt != 0)return;
	gGetSysInfoTcnt = GET_SYSINFO_TIME;
	
	new_temper = Get_AirTemper();			//车内回风温度
	if((new_temper < 100) && (new_temper > -50) && (abs(new_temper - sys_Info.BusInTmp) < 5))
	{
		AirTemper_Buf[sTimes] = new_temper;
		sys_Info.BusInTmpErrTimes = 0;
	}
	else
	{
		AirTemper_Buf[sTimes] = sys_Info.BusInTmp;
		if(sys_Info.BusInTmpErrTimes < 50)sys_Info.BusInTmpErrTimes++;
	}
	
	new_temper = Get_OutTemper();			//车外环境温度
	if((new_temper < 100) && (new_temper > -50) && (abs(new_temper - sys_Info.BusOutTmp) < 5))
	{
		OutTemper_Buf[sTimes] = new_temper;
		sys_Info.BusOutTmpErrTimes = 0;
	}
	else
	{
		OutTemper_Buf[sTimes] = sys_Info.BusOutTmp;
		if(sys_Info.BusOutTmpErrTimes < 50)sys_Info.BusOutTmpErrTimes++;
	}

	new_temper = Get_InPipeTemper();			//内盘管温度
	if((new_temper < 100) && (new_temper > -50) && (abs(new_temper - sys_Info.InPipeTmp) < 5))
	{
		InPipeTemper_Buf[sTimes] = new_temper;
		sys_Info.InPipeTmpErrTimes = 0;
	}
	else
	{
		InPipeTemper_Buf[sTimes] = sys_Info.InPipeTmp;
		if(sys_Info.InPipeTmpErrTimes < 50)sys_Info.InPipeTmpErrTimes++;
	}
			
	new_temper = Get_OutPipeTemper();			//外盘管温度
	if((new_temper < 100) && (new_temper > -50) && (abs(new_temper - sys_Info.OutPipeTmp) < 5))
	{
		OutPipeTemper_Buf[sTimes] = new_temper;
		sys_Info.OutPipeTmpErrTimes = 0;
	}
	else
	{
		OutPipeTemper_Buf[sTimes] = sys_Info.OutPipeTmp;
		if(sys_Info.OutPipeTmpErrTimes < 50)sys_Info.OutPipeTmpErrTimes++;
	}
	
	new_temper = Get_CmpTemper();				//压缩机温度
	if((new_temper < 120) && (new_temper > -50) && (abs(new_temper - sys_Info.CompTmp) < 5))
	{
		CmpTemper_Buf[sTimes] = new_temper;
		sys_Info.CompTmpErrTimes = 0;
	}
	else
	{
		CmpTemper_Buf[sTimes] = sys_Info.CompTmp;
		if(sys_Info.CompTmpErrTimes < 50)sys_Info.CompTmpErrTimes++;
	}
	
	new_temper = Get_StoreTemper();				//控制仓温度
	if((new_temper < 100) && (new_temper > -50) && (abs(new_temper - sys_Info.StoreTmp) < 5))
	{
		StoreTemper_Buf[sTimes] = new_temper;
		sys_Info.StoreTmpErrTimes = 0;
	}
	else
	{
		StoreTemper_Buf[sTimes] = sys_Info.StoreTmp;
		if(sys_Info.StoreTmpErrTimes < 50)sys_Info.StoreTmpErrTimes++;
	}
	
	sTimes++;
	if(sTimes >= 10)
	{
		sys_Info.BusInTmp    = Average_CALC(AirTemper_Buf);
		sys_Info.BusOutTmp   = Average_CALC(OutTemper_Buf);				
		sys_Info.InPipeTmp   = Average_CALC(InPipeTemper_Buf);
		sys_Info.OutPipeTmp  = Average_CALC(OutPipeTemper_Buf);
		sys_Info.StoreTmp    = Average_CALC(StoreTemper_Buf);
		sys_Info.CompTmp     = Average_CALC(CmpTemper_Buf);
		sTimes = 0;
	}
	sys_Info.DCDCVol = Get_Voltage();
}

signed char Get_MCUTemper(void)	//MCU内部温度
{
	uint16 MCUTemperature_ADvalue=0;
	double TemperatureOf25_Value=1143.6032;//(uint16)(1.396/5*4096);
	MCUTemperature_ADvalue=adc_rd(MCUTemper);
	if(MCUTemperature_ADvalue<=TemperatureOf25_Value)
		return (signed char)(25+(((double)(TemperatureOf25_Value-MCUTemperature_ADvalue)/2.9802496)));//3.638 mV/°C	
	else
		return (signed char)(25-(((double)(MCUTemperature_ADvalue-TemperatureOf25_Value)/2.6755072)));//3.266 mV/°C
}

float Temper_CALC(unsigned int Rr, unsigned int B, unsigned char T, unsigned int val)//热敏电阻温度计算
{
	float temp=0.0;
	float x=0.0;
	x = 1.0*4700*val/((4096-val)*Rr);		//分压电阻为4.7kΩ，AD精度12位，Rt = R *EXP(B*(1/T1-1/T2)) ,R=5000,B=3470,T2=273.15+25,Rt为T1（绝对温度）温度时的电阻
	temp = 1.0*B/( (1.0*B/(T+273.15)) + (log(x)/log(E)) )-273.15;
	temp = 0.1*(((signed int)(temp*100 + 5))/10);	//四舍五入，保留一位小数
	return temp;
}

float Get_AirTemper(void)		//车内回风温度
{
	unsigned int airtmp=0;
	airtmp = adc_rd(AirTemper);
	return Temper_CALC(IN_AIR_R, IN_AIR_B, IN_AIR_T, airtmp);
}

float Get_OutTemper(void)		//车外环境温度
{
	unsigned int outtmp=0;
	outtmp = adc_rd(OutTemper);
	return Temper_CALC(OUT_AIR_R, OUT_AIR_B, OUT_AIR_T, outtmp);
}

float Get_InPipeTemper(void)
{
	unsigned int inpipetmp=0;
	inpipetmp = adc_rd(InPipeTemper);
	return Temper_CALC(IN_PIPE_R, IN_PIPE_B, IN_PIPE_T, inpipetmp);
}

float Get_OutPipeTemper(void)
{
	unsigned int outpipetmp=0;
	outpipetmp = adc_rd(OutPipeTemper);
	return Temper_CALC(OUT_PIPE_R, OUT_PIPE_B, OUT_PIPE_T, outpipetmp);
}

float Get_StoreTemper(void)
{
	unsigned int storetmp=0;
	storetmp = adc_rd(StoreTemper);
	return Temper_CALC(STORE_R, STORE_B, STORE_T, storetmp);
}

float Get_CmpTemper(void)
{
	unsigned int cmptmp=0;
	cmptmp = adc_rd(CmpTemper);
	return Temper_CALC(CMP_R, CMP_B, CMP_T, cmptmp);
}

float Get_ReserTemper(void)		//备用温度传感器
{
	unsigned int Reser=0;
	Reser = adc_rd(ReserTemper);
	return Temper_CALC(TEST_R, TEST_B, TEST_T, Reser);
}

float Get_Voltage(void)
{
	unsigned int val;
	float tmp = 0.0;
	float vol = 0.0;
	val = adc_rd(DCVoltage);
	tmp = 55.0*val/4096;	//二极管导通压降0.5V(未加)
	vol = 0.1*(((unsigned int)(tmp*100 + 5))/10);//四舍五入取一位小数
	return vol;
}
