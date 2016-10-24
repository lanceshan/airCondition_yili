/*
 * tactics.h
 *
 *  Created on: Aug 16, 2015
 *      Author: Hill
 */

#ifndef TACTICS_H_
#define TACTICS_H_

unsigned char Tactic_AutoColdInnerFanLv(void);	//自动模式下制冷时内机的风量
unsigned char Tactic_AutoHotInnerFanLv(void);	//自动模式下制热时内机的风量
unsigned char Tactic_ColdCmpLv(void);			//制冷模式时的压缩机工作频率
unsigned char Tactic_HotCmpLv(void);			//制热模式时的压缩机工作频率
unsigned char Tactic_HotInnerFanLv(unsigned char hot_mode);//制热模式下内机的工作频率
unsigned char Tactic_ColdOutsideFanLv(unsigned char cmpLv);	//制冷模式下外机的工作频率
unsigned char Tactic_HotOutsideFanLv(unsigned char cmpLv);	//制热模式下外机的工作频率
unsigned char Tactic_PTC(void);					//PTC控制策略
void Tactic_TmpTrend(void);
#endif /* TACTICS_H_ */
