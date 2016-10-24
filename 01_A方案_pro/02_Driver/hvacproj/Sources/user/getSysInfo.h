/*
 * getSysInfo.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */
#ifndef GETSYSINFO_H_
#define GETSYSINFO_H_

typedef  enum{
	DCVoltage=0,   	//ch6   PTB2
	ReserTemper,	//ch7	PTB3
	InPipeTemper,	//ch8	PTC0
	OutPipeTemper,	//ch9	PTC1
	LeftTemper,		//ch10	PTC2
	RightTemper,	//ch11	PTC3
	CmpTemper,		//ch12	PTF4
	StoreTemper,	//ch13	PTF5
	AirTemper,		//ch14	PTF6
	OutTemper, 		//ch15	PTF7
	MCUTemper		//ch22	IN
}ADC_CHANNEL;

void SysInfo_Init(void);
float Average_CALC(float *buf);
void Get_SysInfo(void);					//获取所有温度数据
signed char Get_MCUTemper(void);		//获取MCU内部温度
float Temper_CALC(unsigned int Rr, unsigned int B, unsigned char T, unsigned int val);//温度计算
float Get_OutTemper(void);				//获取外部温度
float Get_AirTemper(void);				//获取车内温度
float Get_LeftTemper(void);				//获取左吹风温度
float Get_RightTemper(void);			//获取右吹风温度
float Get_InPipeTemper(void);
float Get_OutPipeTemper(void);
float Get_ReserTemper(void);
float Get_StoreTemper(void);
float Get_CmpTemper(void);
float Get_Voltage(void);			//获取24V系统电压

#endif /* GETSYSINFO_H_ */
