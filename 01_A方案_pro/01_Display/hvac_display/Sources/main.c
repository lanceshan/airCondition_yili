/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "clock.h"
#include "timer.h"
#include "commprotocol.h"
#include "eeprom.h"
#include "key.h"
#include "display.h"
#include "globalvars.h"

int main(void)
{
	uint8 key_cmd_flag = 0;
	uint8 UserInfoBuf[8];
	uint8 temp_buf[8] = {0};
	uint8 *pCAN_TxBuf = NULL;
	gUser_Info = (UserMessage *)(UserInfoBuf);
	Set_Buzzer_L;
	Clk_Init();
	GPIO_Init();
	Timer_Init();
	IIC_Init();
	CAN_Init();
	Var_Init();							//ȫ�ֱ�����ʼ��
	Ht1621_Init();
//	Display_Test();
	Read_Prv_Info(&UserInfoBuf[1],3);	//��ȡ��ʷ�������ݣ��������趨�¶ȡ�����ģʽ�������ȼ�

	for(;;) 
	{	
		key_cmd_flag = Key_Handle();
		
		if(key_cmd_flag == NEW_CMD_FAST)gSetOKTime = TIME_SEC / 2;			//����������Ӧʱ��Ϊ0.5s
		if(key_cmd_flag == NEW_CMD_SLOW)gSetOKTime = 5 * TIME_SEC;			//����������Ӧʱ��Ϊ5s
		if(key_cmd_flag != NEW_CMD_NULL)Save_Cur_Info(&UserInfoBuf[1],3);	//�����û�������Ϣ
		
		if(gSetOKTime == 0)
			pCAN_TxBuf = UserInfoBuf;
		else
			pCAN_TxBuf = temp_buf;
		
		if(8 == CAN_Rx())
		{
			CAN_Tx(pCAN_TxBuf);
		}

		Display();
	}
	return 0;
}
