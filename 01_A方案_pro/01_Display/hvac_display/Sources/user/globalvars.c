/*
 * globalvars.c
 *
 *  Created on: Oct 28, 2015
 *      Author: Hill
 */
#include "globalvars.h"

uint8 gBlinkTime;			//��ѯϵͳ�и��¶�ʱ������ͼ����˸ʱ����
uint8 gSkimIndex;			//��ѯϵͳ�и��¶�ʱ�����β�ѯ������
uint8 gSkimFlag;			//�û����ϵͳ���¶ȱ�־
uint8 gSkimTime;			//��ѯ״̬����ʱ��
uint8 gSetOKTime;			//�����������ʱ��
uint8 gKeyPresTime;			//��������ʱ��
uint8 gKeyHandleFlag;		//���������־�����ڳ���ʱ��ÿ400msִ���´ΰ�������
uint8 gCANRxFailTime;		//CANͨ��ʧ�ܼ�ʱ
uint8 gHT1621_RAM[4][13];
SysMessage gSys_Info;		//�����������ϵͳ��Ϣ
UserMessage *gUser_Info;	//�û�������Ϣ
KeyPresHold gKeyPresHold;	//��¼���а���������״̬
KEY_STATE gKeyStatus;		//ʵʱ��¼��ǰ���µİ���

void Var_Init(void)
{
	unsigned int i=0;
	unsigned char j=0;
	
	for(i=0; i<4; i++)
		for(j=0; j<13; j++)
			gHT1621_RAM[i][j] = 0;			//ϵͳ�ϵ�ʱ�����ʾ����
	
	gBlinkTime  = 0;
	gSkimIndex  = 0;
	gSkimFlag   = 0;
	gSkimTime   = 0;
	gSetOKTime  = 0;
	gKeyStatus  = 0;
	gUser_Info->cmd      = 0x33;			//�����û�ָ��������
	gUser_Info->Temper   = 18;				//ϵͳ����Ĭ���趨�¶�18��
	gUser_Info->Mode     = MODE_BLOW;			
	gUser_Info->WindLv   = 1;				//Ĭ�Ϸ���1��
	gUser_Info->FreshAir = FRESH_FAN_OFF;	//Ĭ���·�ر�״̬
	gUser_Info->TestIndex= 0;
	gUser_Info->Power    = POWER_OFF;		//Ĭ�ϵ�Դ�ر�״̬
	gUser_Info->PTC      = 0;				//PTC�ر�
	
	gSys_Info.BusInTmp   = 25.0;
	gSys_Info.InPipeTmp  = 0.0;
	gSys_Info.OutPipeTmp = 0.0;
}
