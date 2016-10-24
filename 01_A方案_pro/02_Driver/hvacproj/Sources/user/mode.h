/*
 * mode.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Hill
 */

#ifndef MODE_H_
#define MODE_H_

#define LEFT_FRESH_MOVE_UP		LeftFresh_Dir_SetVal();
#define LEFT_FRESH_MOVE_DOWN	LeftFresh_Dir_ClrVal();
#define LEFT_FRESH_ENABLE		LeftFresh_Enable_SetVal();
#define LEFT_FRESH_DISABLE		LeftFresh_Enable_ClrVal();

typedef void (*tModeProcess)(void);

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

void HaltModeProcess(void);
void AutoModeProcess(void);
void ColdModeProcess(void);
void HotModeProcess(void);
void BlowerModeProcess(void);
void LowPowerModeProcess(void);
void HomeModeProcess(void);
void TestModeProcess(void);
void PauseModeProcess(void);
unsigned char FrostModeProcess(void);
void Fresh_Fan_Ctrl(unsigned char fan_state);
DRV_MODE Mode_Switch(DRV_MODE cur_Mode);

void ModeProcess(DRV_MODE cur_mode);
unsigned char Power_OFFProcess(unsigned char offstep);
#endif /* MODE_H_ */
