/*
 * tactics.h
 *
 *  Created on: Aug 16, 2015
 *      Author: Hill
 */

#ifndef TACTICS_H_
#define TACTICS_H_

unsigned char Tactic_AutoColdInnerFanLv(void);	//�Զ�ģʽ������ʱ�ڻ��ķ���
unsigned char Tactic_AutoHotInnerFanLv(void);	//�Զ�ģʽ������ʱ�ڻ��ķ���
unsigned char Tactic_ColdCmpLv(void);			//����ģʽʱ��ѹ��������Ƶ��
unsigned char Tactic_HotCmpLv(void);			//����ģʽʱ��ѹ��������Ƶ��
unsigned char Tactic_HotInnerFanLv(unsigned char hot_mode);//����ģʽ���ڻ��Ĺ���Ƶ��
unsigned char Tactic_ColdOutsideFanLv(unsigned char cmpLv);	//����ģʽ������Ĺ���Ƶ��
unsigned char Tactic_HotOutsideFanLv(unsigned char cmpLv);	//����ģʽ������Ĺ���Ƶ��
unsigned char Tactic_PTC(void);					//PTC���Ʋ���
void Tactic_TmpTrend(void);
#endif /* TACTICS_H_ */
