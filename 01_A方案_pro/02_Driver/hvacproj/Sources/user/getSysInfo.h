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
void Get_SysInfo(void);					//��ȡ�����¶�����
signed char Get_MCUTemper(void);		//��ȡMCU�ڲ��¶�
float Temper_CALC(unsigned int Rr, unsigned int B, unsigned char T, unsigned int val);//�¶ȼ���
float Get_OutTemper(void);				//��ȡ�ⲿ�¶�
float Get_AirTemper(void);				//��ȡ�����¶�
float Get_LeftTemper(void);				//��ȡ�󴵷��¶�
float Get_RightTemper(void);			//��ȡ�Ҵ����¶�
float Get_InPipeTemper(void);
float Get_OutPipeTemper(void);
float Get_ReserTemper(void);
float Get_StoreTemper(void);
float Get_CmpTemper(void);
float Get_Voltage(void);			//��ȡ24Vϵͳ��ѹ

#endif /* GETSYSINFO_H_ */
