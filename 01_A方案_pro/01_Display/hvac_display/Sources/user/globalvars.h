/*
 * globalvars.h
 *
 *  Created on: Oct 28, 2015
 *      Author: Hill
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include <typedefs.h>
#include <null.h>

#define TIME_SEC				10				//10*100ms = 1s
#define KEY_NEXT_CHECK_TIME		4				//KEY_HOLD_TIME*100ms
#define	KEY_HOLD_TIME			15				//��������1.5s�����������������ܣ���ҪӦ���ڲ�ѯ��
#define NEW_CMD_NULL			0				//�ް�������
#define NEW_CMD_SLOW			1				//������Ӧ��������
#define NEW_CMD_FAST			2				//������Ӧ��������
#define SKIM_TIME				(5*TIME_SEC)	//��ѯģʽ����ʱ5s��5sû�в�����ת������ģʽ
#define SKIM_TEMPER_NUM			6				//��ѯ�¶�����
#define MAX_CAN_FAIL_TIME		(5*TIME_SEC)	//ͨ��ʧ��5s��ͨ�Ź��ϱ���
#define MAX_SET_TEMPER			30				//�û���������¶�
#define MIN_SET_TEMPER			16				//�û���С�����¶�
#define MAX_BLOWER_Lv			6				//�û�������÷���
#define MIN_BLOWER_Lv			1				//�û���С���÷���
#define POWER_ON				1
#define POWER_OFF				0
#define FRESH_FAN_HIGH			2				//�·�ȫ��
#define FRESH_FAN_LOW			1				//�·�뿪
#define FRESH_FAN_OFF			0				//�·�ر�
#define BMP_BLINK_OFF			0				//��ʾ���ͼ��ر�
#define BMP_BLINK_ON			6				//��ʾ���ͼ��ȫ��
#define BLINK_FRQ_TIME			((TIME_SEC) / 2)//������ʾ��˸Ƶ��
#define ERR_E					20				//������ʾ��E��
#define ERR_R					21				//������ʾ��R��

typedef enum{
	MODE_HALT=0,
	MODE_COLD,		//����ģʽ
	MODE_HOT,		//����ģʽ
	MODE_BLOW,		//����ģʽ
	MODE_AUTO, 		//�Զ�ģʽ
	MODE_TEST,		//���ģʽ
	MODE_HOME,		//Ӧ��ģʽ
	MODE_PAUSE,		//��ͣģʽ����
}DRV_MODE;

typedef struct{
	unsigned char Cmd;			//ϵͳ״̬�����������
	float BusInTmp;				//��ǰ���ڻط��¶�
	float BusOutTmp;			//��ǰ�󴵷��¶�
	float InPipeTmp;			//��˪�¶�
	float OutPipeTmp;
	float StoreTmp;				//�������¶�
	float CompTmp;				//ѹ�����¶�
	float MCUTmp;				//MCU�ڲ��¶�
	unsigned char BusInTmpSign;		//�ط��¶���������
	unsigned char BusOutTmpSign;
	unsigned char InPipeTmpSign;
	unsigned char OutPipeTmpSign;
	unsigned char StoreTmpSign;
	unsigned char CompTmpSign;
	unsigned char MCUTmpSign;
//	float DCVoltage;			//ϵͳ24V��ѹ
	unsigned char Mode;			//ϵͳ��ǰ����ģʽ
	unsigned char WindLv;		//ϵͳ��ǰ����
	unsigned char FreshAir;		//ϵͳ��ǰ�·�״̬������1���أ�0
	union {
		unsigned char Byte;
		struct{
			unsigned char CompOverPres	:1;	//ѹ������ѹ
			unsigned char CompLowPres	:1;	//ѹ������ѹ
			unsigned char VFDFault		:1;	//��Ƶ������
			unsigned char OutsideFan	:1;	//����������
			unsigned char InnerFan 		:1;	//�ڻ��������
			unsigned char CANFault		:1;	//CANͨ�Ź���
			unsigned char LeftFresh 	:1;	//����·���
			unsigned char RightFresh 	:1;	//�Ҳ��·���
		} bits;						
	}Errorflags;
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
	DRV_MODE      Mode;			//�û����õ�ģʽ
	unsigned char WindLv;		//�û����õķ����ȼ�
	unsigned char FreshAir;		//�û����õ��·�״̬
	unsigned char PTC;
	unsigned char TestIndex;	//���ԣ�1��ѹ�������䣬2��ѹ�������ȣ�3��PTC����
	unsigned char Power;
}UserMessage;

typedef enum{
	Key_Null = 0,
	Key_Power,
	Key_Mode,
	Key_Auto,
	Key_Skim,
	Key_TemperPlus,
	Key_TemperMinus,
	Key_WindPlus,
	Key_WindMinus,
	Key_PTC,
	Key_FreshAir
}KEY_STATE;

typedef union{
	unsigned char Flags;
	struct{
		unsigned char Power 		:1;
		unsigned char Mode			:1;	
		unsigned char Auto			:1;	
		unsigned char Skim			:1;	
		unsigned char TemperPlus 	:1;
		unsigned char TemperMinus	:1;	
		unsigned char WindPlus		:1;	
		unsigned char WindMinus		:1;
	} bits;	
}KeyPresHold;

extern SysMessage  gSys_Info;		//�����������ϵͳ��Ϣ
extern UserMessage *gUser_Info;		//�û�������Ϣ
extern KeyPresHold gKeyPresHold;	//��¼���а���������״̬
extern KEY_STATE gKeyStatus;		//ʵʱ��¼��ǰ���µİ���
extern uint8 gSkimTime;				//��ѯ״̬����ʱ��
extern uint8 gSkimFlag;				//�û���ѯģʽ��־
extern uint8 gSkimIndex;			//��ѯϵͳ�и��¶�ʱ�����β�ѯ������
extern uint8 gBlinkTime;			//��ѯϵͳ�и��¶�ʱ������ͼ����˸ʱ����
extern uint8 gSetOKTime;			//�����������ʱ��
extern uint8 gKeyPresTime;			//��������ʱ��
extern uint8 gKeyHandleFlag;		//���������־�����ڳ���ʱ��ÿ400msִ���´ΰ�������
extern uint8 gCANRxFailTime;		//CANͨ��ʧ�ܼ�ʱ
extern uint8 gHT1621_RAM[4][13];
void Var_Init(void);

#endif /* GLOBALVARS_H_ */
