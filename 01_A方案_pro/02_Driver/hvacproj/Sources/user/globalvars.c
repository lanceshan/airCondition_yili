/*
 * globalvars.c
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#include "globalvars.h"
//ͨ�����
unsigned char CANRx_Buf[8]={0};				//CAN���ջ���
unsigned char CANTx_Buf[8]={0};				//CAN���ͻ���
unsigned long CAN_ID = 0;

//��ʱ�����
unsigned int  gSysStartTcnt = 0;			//ϵͳ�ϵ�����5s��ʱ����
unsigned int  gMsgTxTcnt = 0; 				//CAN��Ϣ����ʱ����500ms
unsigned int  gSysRunTcnt = TIME_SEC; 		//ϵͳ���м��1s��ʱ����
unsigned int  gColdToHotTcnt = 0;			//�����л�������ģʽ��ʱ3min��ʱ����
unsigned int  gHotToColdTcnt = 0;			//�����л�������ģʽ��ʱ1min��ʱ����
unsigned int  gGetSysInfoTcnt = 0;			//�ɼ��¶�ʱ����
unsigned int  gTrendChgTcnt = 0;			//�ж��¶ȱ仯����ʱ����5s
unsigned int  gOFFModeTcnt = 0;				//�ػ�ģʽ��ʱ
unsigned int  gDCErrCheckTcnt = 0;			//DCDC������ʱ���ʱ��
unsigned int  gPowerErrCheckTcnt = 0;		//�ܵ������ʱ���ʱ��
unsigned int  gDefrostTcnt = 0;				//��˪״̬��ʱ
unsigned int  gFrostLowTempTcnt = 0;		//��˪�¶ȴ��������¼�ʱ
unsigned int  gRightFreshWks_Tcnt = 0;		//�·��ŵ������ʱ��
unsigned int  gLeftFreshWks_Tcnt = 0;
DRV_STATE 	  gcur_state = state_halt;
UserMessage   user_Info;
SysMessage    sys_Info;
CtrlMessage   sys_Ctrl;
