/*
 * globalvars.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

//#define Debug

#define TIME_SEC			100
#define TIME_MIN			(60*TIME_SEC)
#define TIME_SYS_START		(2*TIME_SEC)		//ϵͳ����2s��ʱ
#define TIME_MSG_SEND		TIME_SEC/2			//ͨ��ʱ����500ms
#define HOT_TO_COLD_TIME 	TIME_MIN 			//�����л�������ʱ���� 60s
#define COLD_TO_HOT_TIME 	(3*TIME_MIN)		//�����л�������ʱ���� 180s
#define PTC_Wks_TEMPER 		-5					//ֻ��PTC�������¶���ֵ
#define PTC_STOP_TEMPER 	8					//��ֹPTC�������¶���ֵ
#define GET_SYSINFO_TIME    (TIME_SEC/2)		//�ɼ�ϵͳ��Ϣʱ����500ms
#define TREND_CHANGE_TIME	(5*TIME_SEC)		//�¶����Ʊ仯ʱ����5s
#define FAN_LOW_FRQ_TIME	(2*TIME_SEC)		//����������Ƶ����ʱ�䣬2s
#define OFF_MODE_TIME		(36*TIME_SEC)		//�ػ�ģʽʱ���ʱ18s
#define AUTO_COLD_TEMPER    25					//�Զ������¶���ֵ
#define AUTO_HOT_TEMPER		15					//�Զ������¶���ֵ
#define TEMP_OFFSET			0					//�ɼ����¶�����ƫ��ֵ�����Բ���ʱʹ��
#define FRESH_FAN_WK_TIME	(5*1000)			//�·��ŵ�������ʱ��5s
#define FRESH_POS_MOVE		0
#define FRESH_POS_CLOSE		1
#define FRESH_POS_MIDDLE	2
#define FRESH_POS_OPEN		3
#define FRESH_FAN_OFF		0
#define FRESH_FAN_LOW		1
#define FRESH_FAN_HIGH		2
#define POWER_ON			1
#define POWER_OFF			0
//ͨ�����
extern unsigned char CANRx_Buf[8];			//CAN���ջ���
extern unsigned char CANTx_Buf[8];			//CAN���ͻ���
extern unsigned long CAN_ID;

//ʱ�����
extern unsigned int  gSysStartTcnt; 		//ϵͳ�ϵ�����2s��ʱ����
extern unsigned int  gMsgTxTcnt; 			//CAN����ʱ����500ms��ʱ����
extern unsigned int  gSysRunTcnt; 			//ϵͳ���м��1s��ʱ����
extern unsigned int  gColdToHotTcnt;		//�����л�������ģʽ��ʱ3min��ʱ����
extern unsigned int  gHotToColdTcnt;		//�����л�������ģʽ��ʱ1min��ʱ����
extern unsigned int  gGetSysInfoTcnt;		//�ɼ�ϵͳ��Ϣʱ����500ms
extern unsigned int  gTrendChgTcnt;			//�ж��¶ȱ仯����ʱ����5s
extern unsigned int  gOFFModeTcnt;			//�ػ�ģʽ��ʱ
extern unsigned int  gDCErrCheckTcnt;		//DCDC������ʱ���ʱ��
extern unsigned int  gPowerErrCheckTcnt;	//�ܵ������ʱ���ʱ��
extern unsigned int  gDefrostTcnt;			//��˪״̬��ʱ
extern unsigned int  gFrostLowTempTcnt;		//��˪�¶ȴ��������¼�ʱ
extern unsigned int  gRightFreshWks_Tcnt;	//�·��ŵ������ʱ��
extern unsigned int  gLeftFreshWks_Tcnt;

typedef struct{
	unsigned char Cmd;			//ϵͳ״̬�����������
	float BusInTmp;				//���ڻط��¶�
	float BusOutTmp;			//���⻷���¶�
	float InPipeTmp;			//���̹��¶�
	float OutPipeTmp;			//���̹��¶�
	float CompTmp;				//ѹ�����¶�
	float StoreTmp;				//���������¶�
	float MCUTmp;				//MCU�ڲ��¶�
	float DCDCVol;				//ϵͳ24V��ѹ
	unsigned char BusInTmpErrTimes;
	unsigned char BusOutTmpErrTimes;
	unsigned char InPipeTmpErrTimes;
	unsigned char OutPipeTmpErrTimes;
	unsigned char CompTmpErrTimes;
	unsigned char StoreTmpErrTimes;
	unsigned char CANRecvErrTimes;		//CANͨ�Ž���ʧ�ܼ���
	union {
		unsigned char Byte;
		struct{
			unsigned char CompOverPres	:1;	//ѹ������ѹ
			unsigned char CompLowPres	:1;	//ѹ������ѹ
			unsigned char VFDFault		:1;	//��Ƶ������
			unsigned char OutsideFan	:1;	//���������ϣ�Ŀǰû�ж����������й��ϼ��
			unsigned char InnerFan 		:1;	//�ڻ��������
			unsigned char CANFault		:1;	//CANͨ�Ź���
			unsigned char LeftFresh 	:1;	//����·���
			unsigned char RightFresh 	:1;	//�Ҳ��·���
		} bits;						
	}ErrDevices;
	union {
		unsigned char Byte;
		struct{
			unsigned char TravSwitch 	:1;	//�г̿���
			unsigned char PowerOver 	:1;	//������ѹ
			unsigned char PowerLow	 	:1;	//������ѹ
			unsigned char DCDCOver		:1;	//DCDC�����ѹ
			unsigned char DCDCLow		:1;	//DCDC���Ƿѹ
		} bits;						
	}ErrPower;
	union {
		unsigned char Byte;
		struct{
			unsigned char BusIn 		:1;	//�����¶ȴ�����ʧЧ
			unsigned char BusOut		:1;	//���⻷���¶ȴ�����ʧЧ
			unsigned char InPipe		:1;	//���̹��¶ȴ�����ʧЧ
			unsigned char OutPipe		:1;	//���̹��¶ȴ�����ʧЧ
			unsigned char Store 		:1;	//���Ʋ��¶ȴ�����ʧЧ
			unsigned char CompHeat		:1;	//ѹ��������
		} bits;	
	}ErrTempers;
}SysMessage;

typedef struct{	
	unsigned char cmd;
	unsigned char Temper;		//�û����õ��¶�
	unsigned char Mode;			//�û����õ�ģʽ
	unsigned char WindLv;		//�û����õķ����ȼ�
	unsigned char FreshAir;		//�û����õ��·�״̬
	unsigned char PTC;
	unsigned char TestIndex;	//���ԣ�1��ѹ�������䣬2��ѹ�������ȣ�3��PTC����
	unsigned char Power;
}UserMessage;

typedef struct{	
	unsigned char 		CompLv;			//ѹ����������λ
	unsigned char 		CompOffEn;		//ѹ�����ر�ʹ��
	unsigned char 		CompOnEn;		//ѹ������ʹ��
	unsigned char 		CompHiLvEn;		//ѹ��������ʹ��
	unsigned int  		CompOffTime;	//ѹ�����ر�ʱ��
	unsigned int  		CompOnTime;		//ѹ��������ʱ��
	unsigned int  		CompHiLvTime;	//ѹ����60Hz��5��������ʱ��	
	unsigned int  		CompOffStepTime;//ѹ�����ػ�Ƶ�ʽ���ʱ����
	unsigned long int  	CompHotTime;	//ѹ������������ʱ��
	unsigned char 		InnerFanLv;		//�ڻ�������λ
	unsigned char 		InnerFanEn;		//�ڻ�����ʹ��
	unsigned int  		InnerFanOnTime;	//�ڻ�����ʱ��
	unsigned char 		OutsideFanLv;	//���������λ
	unsigned int  		OutsideFanOnTime;//�������ʱ��
	unsigned char 		PTCLv;			//PTC������λ
}CtrlMessage;

typedef enum{
	state_halt=0,		//��״̬
	state_compcold, 	//ѹ��������״̬
	state_comphot,		//ѹ��������
	state_ptchot,		//ptc����
	state_bothhot,		//ѹ������ptcͬʱ����
	state_blow,			//����״̬
	state_frost,		//��˪״̬
}DRV_STATE;

extern DRV_STATE gcur_state;
extern UserMessage user_Info;
extern SysMessage sys_Info;
extern CtrlMessage sys_Ctrl;

#endif /* GLOBALVARS_H_ */
