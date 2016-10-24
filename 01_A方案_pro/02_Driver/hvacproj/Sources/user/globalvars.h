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
#define TIME_SYS_START		(2*TIME_SEC)		//系统启动2s计时
#define TIME_MSG_SEND		TIME_SEC/2			//通信时间间隔500ms
#define HOT_TO_COLD_TIME 	TIME_MIN 			//制热切换到制冷时间间隔 60s
#define COLD_TO_HOT_TIME 	(3*TIME_MIN)		//制冷切换到制热时间间隔 180s
#define PTC_Wks_TEMPER 		-5					//只有PTC工作的温度阈值
#define PTC_STOP_TEMPER 	8					//禁止PTC工作的温度阈值
#define GET_SYSINFO_TIME    (TIME_SEC/2)		//采集系统信息时间间隔500ms
#define TREND_CHANGE_TIME	(5*TIME_SEC)		//温度趋势变化时间间隔5s
#define FAN_LOW_FRQ_TIME	(2*TIME_SEC)		//风机启动后低频工作时间，2s
#define OFF_MODE_TIME		(36*TIME_SEC)		//关机模式时间计时18s
#define AUTO_COLD_TEMPER    25					//自动制冷温度阈值
#define AUTO_HOT_TEMPER		15					//自动制热温度阈值
#define TEMP_OFFSET			0					//采集的温度数据偏置值，调试参数时使用
#define FRESH_FAN_WK_TIME	(5*1000)			//新风门单次运行时间5s
#define FRESH_POS_MOVE		0
#define FRESH_POS_CLOSE		1
#define FRESH_POS_MIDDLE	2
#define FRESH_POS_OPEN		3
#define FRESH_FAN_OFF		0
#define FRESH_FAN_LOW		1
#define FRESH_FAN_HIGH		2
#define POWER_ON			1
#define POWER_OFF			0
//通信相关
extern unsigned char CANRx_Buf[8];			//CAN接收缓存
extern unsigned char CANTx_Buf[8];			//CAN发送缓存
extern unsigned long CAN_ID;

//时间管理
extern unsigned int  gSysStartTcnt; 		//系统上电启动2s定时计数
extern unsigned int  gMsgTxTcnt; 			//CAN发送时间间隔500ms定时计数
extern unsigned int  gSysRunTcnt; 			//系统运行间隔1s定时计数
extern unsigned int  gColdToHotTcnt;		//制冷切换到制热模式延时3min定时计数
extern unsigned int  gHotToColdTcnt;		//制热切换到制冷模式延时1min定时计数
extern unsigned int  gGetSysInfoTcnt;		//采集系统信息时间间隔500ms
extern unsigned int  gTrendChgTcnt;			//判断温度变化趋势时间间隔5s
extern unsigned int  gOFFModeTcnt;			//关机模式计时
extern unsigned int  gDCErrCheckTcnt;		//DCDC故障延时检测时间
extern unsigned int  gPowerErrCheckTcnt;	//总电故障延时检测时间
extern unsigned int  gDefrostTcnt;			//除霜状态计时
extern unsigned int  gFrostLowTempTcnt;		//除霜温度传感器低温计时
extern unsigned int  gRightFreshWks_Tcnt;	//新风门电机工作时间
extern unsigned int  gLeftFreshWks_Tcnt;

typedef struct{
	unsigned char Cmd;			//系统状态返回命令功能码
	float BusInTmp;				//车内回风温度
	float BusOutTmp;			//车外环境温度
	float InPipeTmp;			//内盘管温度
	float OutPipeTmp;			//外盘管温度
	float CompTmp;				//压缩机温度
	float StoreTmp;				//电气仓内温度
	float MCUTmp;				//MCU内部温度
	float DCDCVol;				//系统24V电压
	unsigned char BusInTmpErrTimes;
	unsigned char BusOutTmpErrTimes;
	unsigned char InPipeTmpErrTimes;
	unsigned char OutPipeTmpErrTimes;
	unsigned char CompTmpErrTimes;
	unsigned char StoreTmpErrTimes;
	unsigned char CANRecvErrTimes;		//CAN通信接收失败计数
	union {
		unsigned char Byte;
		struct{
			unsigned char CompOverPres	:1;	//压缩机过压
			unsigned char CompLowPres	:1;	//压缩机低压
			unsigned char VFDFault		:1;	//变频器故障
			unsigned char OutsideFan	:1;	//外机风机故障，目前没有对内外风机进行故障监测
			unsigned char InnerFan 		:1;	//内机风机故障
			unsigned char CANFault		:1;	//CAN通信故障
			unsigned char LeftFresh 	:1;	//左侧新风门
			unsigned char RightFresh 	:1;	//右侧新风门
		} bits;						
	}ErrDevices;
	union {
		unsigned char Byte;
		struct{
			unsigned char TravSwitch 	:1;	//行程开关
			unsigned char PowerOver 	:1;	//整车高压
			unsigned char PowerLow	 	:1;	//整车低压
			unsigned char DCDCOver		:1;	//DCDC输出过压
			unsigned char DCDCLow		:1;	//DCDC输出欠压
		} bits;						
	}ErrPower;
	union {
		unsigned char Byte;
		struct{
			unsigned char BusIn 		:1;	//车内温度传感器失效
			unsigned char BusOut		:1;	//车外环境温度传感器失效
			unsigned char InPipe		:1;	//内盘管温度传感器失效
			unsigned char OutPipe		:1;	//外盘管温度传感器失效
			unsigned char Store 		:1;	//控制仓温度传感器失效
			unsigned char CompHeat		:1;	//压缩机过热
		} bits;	
	}ErrTempers;
}SysMessage;

typedef struct{	
	unsigned char cmd;
	unsigned char Temper;		//用户设置的温度
	unsigned char Mode;			//用户设置的模式
	unsigned char WindLv;		//用户设置的风量等级
	unsigned char FreshAir;		//用户设置的新风状态
	unsigned char PTC;
	unsigned char TestIndex;	//测试，1：压缩机制冷，2：压缩机制热，3：PTC制热
	unsigned char Power;
}UserMessage;

typedef struct{	
	unsigned char 		CompLv;			//压缩机工作档位
	unsigned char 		CompOffEn;		//压缩机关闭使能
	unsigned char 		CompOnEn;		//压缩机打开使能
	unsigned char 		CompHiLvEn;		//压缩机高速使能
	unsigned int  		CompOffTime;	//压缩机关闭时间
	unsigned int  		CompOnTime;		//压缩机工作时间
	unsigned int  		CompHiLvTime;	//压缩机60Hz（5档）工作时间	
	unsigned int  		CompOffStepTime;//压缩机关机频率降低时间间隔
	unsigned long int  	CompHotTime;	//压缩机持续制热时间
	unsigned char 		InnerFanLv;		//内机工作档位
	unsigned char 		InnerFanEn;		//内机工作使能
	unsigned int  		InnerFanOnTime;	//内机工作时间
	unsigned char 		OutsideFanLv;	//外机工作档位
	unsigned int  		OutsideFanOnTime;//外机工作时间
	unsigned char 		PTCLv;			//PTC工作档位
}CtrlMessage;

typedef enum{
	state_halt=0,		//空状态
	state_compcold, 	//压缩机制冷状态
	state_comphot,		//压缩机制热
	state_ptchot,		//ptc制热
	state_bothhot,		//压缩机和ptc同时工作
	state_blow,			//吹风状态
	state_frost,		//除霜状态
}DRV_STATE;

extern DRV_STATE gcur_state;
extern UserMessage user_Info;
extern SysMessage sys_Info;
extern CtrlMessage sys_Ctrl;

#endif /* GLOBALVARS_H_ */
