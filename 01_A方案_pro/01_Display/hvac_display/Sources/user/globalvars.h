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
#define	KEY_HOLD_TIME			15				//按键长按1.5s，开启按键长按功能，主要应用于查询键
#define NEW_CMD_NULL			0				//无按键按下
#define NEW_CMD_SLOW			1				//慢速响应按键按下
#define NEW_CMD_FAST			2				//快速响应按键按下
#define SKIM_TIME				(5*TIME_SEC)	//查询模式倒计时5s，5s没有操作跳转到正常模式
#define SKIM_TEMPER_NUM			6				//查询温度数量
#define MAX_CAN_FAIL_TIME		(5*TIME_SEC)	//通信失败5s后，通信故障报错
#define MAX_SET_TEMPER			30				//用户最大设置温度
#define MIN_SET_TEMPER			16				//用户最小设置温度
#define MAX_BLOWER_Lv			6				//用户最大设置风量
#define MIN_BLOWER_Lv			1				//用户最小设置风量
#define POWER_ON				1
#define POWER_OFF				0
#define FRESH_FAN_HIGH			2				//新风全开
#define FRESH_FAN_LOW			1				//新风半开
#define FRESH_FAN_OFF			0				//新风关闭
#define BMP_BLINK_OFF			0				//显示面板图标关闭
#define BMP_BLINK_ON			6				//显示面板图标全开
#define BLINK_FRQ_TIME			((TIME_SEC) / 2)//故障显示闪烁频率
#define ERR_E					20				//错误显示“E”
#define ERR_R					21				//错误显示“R”

typedef enum{
	MODE_HALT=0,
	MODE_COLD,		//制冷模式
	MODE_HOT,		//制热模式
	MODE_BLOW,		//吹风模式
	MODE_AUTO, 		//自动模式
	MODE_TEST,		//检测模式
	MODE_HOME,		//应急模式
	MODE_PAUSE,		//暂停模式处理
}DRV_MODE;

typedef struct{
	unsigned char Cmd;			//系统状态返回命令功能码
	float BusInTmp;				//当前车内回风温度
	float BusOutTmp;			//当前左吹风温度
	float InPipeTmp;			//除霜温度
	float OutPipeTmp;
	float StoreTmp;				//机舱内温度
	float CompTmp;				//压缩机温度
	float MCUTmp;				//MCU内部温度
	unsigned char BusInTmpSign;		//回风温度正负符号
	unsigned char BusOutTmpSign;
	unsigned char InPipeTmpSign;
	unsigned char OutPipeTmpSign;
	unsigned char StoreTmpSign;
	unsigned char CompTmpSign;
	unsigned char MCUTmpSign;
//	float DCVoltage;			//系统24V电压
	unsigned char Mode;			//系统当前工作模式
	unsigned char WindLv;		//系统当前风量
	unsigned char FreshAir;		//系统当前新风状态，开：1，关：0
	union {
		unsigned char Byte;
		struct{
			unsigned char CompOverPres	:1;	//压缩机过压
			unsigned char CompLowPres	:1;	//压缩机低压
			unsigned char VFDFault		:1;	//变频器故障
			unsigned char OutsideFan	:1;	//外机风机故障
			unsigned char InnerFan 		:1;	//内机风机故障
			unsigned char CANFault		:1;	//CAN通信故障
			unsigned char LeftFresh 	:1;	//左侧新风门
			unsigned char RightFresh 	:1;	//右侧新风门
		} bits;						
	}Errorflags;
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
	DRV_MODE      Mode;			//用户设置的模式
	unsigned char WindLv;		//用户设置的风量等级
	unsigned char FreshAir;		//用户设置的新风状态
	unsigned char PTC;
	unsigned char TestIndex;	//测试，1：压缩机制冷，2：压缩机制热，3：PTC制热
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

extern SysMessage  gSys_Info;		//接收驱动板的系统信息
extern UserMessage *gUser_Info;		//用户设置信息
extern KeyPresHold gKeyPresHold;	//记录所有按键长按的状态
extern KEY_STATE gKeyStatus;		//实时记录当前按下的按键
extern uint8 gSkimTime;				//查询状态持续时间
extern uint8 gSkimFlag;				//用户查询模式标志
extern uint8 gSkimIndex;			//查询系统中各温度时，依次查询索引号
extern uint8 gBlinkTime;			//查询系统中各温度时，风量图标闪烁时间间隔
extern uint8 gSetOKTime;			//按键操作完成时间
extern uint8 gKeyPresTime;			//按键按下时间
extern uint8 gKeyHandleFlag;		//按键处理标志，用在长按时，每400ms执行下次按键处理
extern uint8 gCANRxFailTime;		//CAN通信失败计时
extern uint8 gHT1621_RAM[4][13];
void Var_Init(void);

#endif /* GLOBALVARS_H_ */
