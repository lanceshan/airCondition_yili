/*
 * can.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
#include "can.h"
#include "msCANdrv.h"
#include "derivative.h"

void CAN_Init(void)
{
	uint8 err_status;
	uint8 CAN_status[3] = {0};						    //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
	MSCAN_ModuleEn();
	EnableInterrupts;
	 
	err_status = Init_CAN(0, CMPTX);		        //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
	 
	while ((CAN_status[0] & SYNCH) == 0) 	     	//Wait for msCAN channel 0 synchronized to CAN Bus
	{	
	    err_status = Check_CAN_Status(0, CAN_status);
	}
	    
	err_status = Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "1" means the message to send out with ID:0x81;                                               //transmit is the message object 1 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x81,Can be modified;
	err_status = Config_CAN_MB (0, 0, RXDF, 0);	//Configure the msCAN channel 0 buffer 0 to be RXDF mode (receive dataframe),"0" means to receive the specific frame with ID:0x80
	
	err_status = err_status;
}
void MSCAN_Rx_IRQHandler(void)
{
	CAN_Receive(0);
}
void MSCAN_Tx_IRQHandler(void)
{
	if(MSCAN_CANTFLG & MSCAN_CANTIER)
		CAN_Transmit(0);
	if((MSCAN_CANRFLG&0x02)||((MSCAN_CANRFLG>>6)&0x01))
		CAN_ERR(0);
	if(MSCAN_CANRFLG &0x80)
		CAN_Wakeup(0);
}

void MSCAN_ModuleEn(void)
{
  /*Enable MSCAN Module*/
  SIM_PINSEL1 &= (uint32_t)~(uint32_t)(SIM_PINSEL1_MSCANPS_MASK);/* SIM_PINSEL1: MSCANPS=0 *///PTC6 and PTC7
  SIM_SCGC |=(uint32_t)(SIM_SCGC_MSCAN_MASK);/* SIM_SCGC: MSCAN=1 */ //CAN clock enable
  /* NVIC_ICER: CLRENA31=0,CLRENA30=0,CLRENA29=0,CLRENA28=0,CLRENA27=0,CLRENA26=0,CLRENA25=0,CLRENA24=0,CLRENA23=0,CLRENA22=0,CLRENA21=0,CLRENA20=0,CLRENA19=0,CLRENA18=0,CLRENA17=0,CLRENA16=0,CLRENA15=0,CLRENA14=0,CLRENA13=0,CLRENA12=0,CLRENA11=0,CLRENA10=0,CLRENA9=0,CLRENA8=0,CLRENA7=0,CLRENA6=0,CLRENA5=0,CLRENA4=0,CLRENA3=0,CLRENA2=0,CLRENA1=0,CLRENA0=0 */
  NVIC_ICER = 0x00U;
  /* NVIC_IPR7: PRI_31=1,PRI_30=1 */
  NVIC_IPR7 = (uint32_t)((NVIC_IPR7 & (uint32_t)~(uint32_t)(
	 NVIC_IP_PRI_31(0x02) |
	 NVIC_IP_PRI_30(0x02)
	)) | (uint32_t)(
	 NVIC_IP_PRI_31(0x01) |
	 NVIC_IP_PRI_30(0x01)
	));
  /* NVIC_ISER: SETENA31=1,SETENA30=1,SETENA29=0,SETENA28=0,SETENA27=0,SETENA26=0,SETENA25=0,SETENA24=0,SETENA23=0,SETENA22=0,SETENA21=0,SETENA20=0,SETENA19=0,SETENA18=0,SETENA17=0,SETENA16=0,SETENA15=0,SETENA14=0,SETENA13=0,SETENA12=0,SETENA11=0,SETENA10=0,SETENA9=0,SETENA8=0,SETENA7=0,SETENA6=0,SETENA5=0,SETENA4=0,SETENA3=0,SETENA2=0,SETENA1=0,SETENA0=0 */
  NVIC_ISER = (SIM_SCGC_ACMP0_MASK | SIM_SCGC_ACMP1_MASK);
  	  	  	  	  	//(0x40000000u | 0x80000000u)
}

void Transmit_DataFrame(uint8 len, uint8 *Txbuf) 
{
	uint8 err_status;    //To store the return value of the driver APIs.
	
	/*the first number of the array "6" means the transmitted data length of
	the transmitted data,the MAX datalength is 8, the rest numbers are the data
	bytes to transmit.*/
	 EnableInterrupts;
	 
	 err_status = Init_CAN(0,CMPTX);		         //Initialization msCAN channel 0, FAST means the reset won't wait until current transmission completed 
	 
	 err_status = Config_CAN_MB (0, 1, TXDF, 1); //Configure the msCAN channel 0 buffer 1 to be TXDF mode (transmit dataframe), "0" means the message to 
	                                             //transmit is the message object 0 of msCAN channel 0,see details in "MSCANID.h",So ID is 0x80 which can be modified;
	err_status = Load_CAN_MB(0, 1, len, Txbuf);		 //Load the data to the buffer of Channel 0 buffer 1
	
	err_status = Transmit_CAN_MB(0, 1);		 //Transmit the frame of Channel 0 buffer 1;
	err_status = err_status;                     /*prevent compiling warning*/
}

uint8 Receive_DataFrame(uint8 *data_rec) 
{ 
    uint8 err_status = 0;    				//To store the return value of the driver APIs.
    uint8 datalen = 0;                         //The first number of data_rec is the datalenth, the max data length is 8, so the max length of data_rec should be 9
    uint8 buffer_status[2] = {0};						//the array get the required buffer status,the first byte is status,the second byte is ID Ref			    
    uint16 i = 10000;
    uint8 CAN_status[3];						    //the array get the required msCAN Module status, the first byte CANCTL0, second CANCTL1, third CANRFLG
				    
    EnableInterrupts;
 
    err_status = Init_CAN(0, CMPTX);		        //Initialization msCAN channel 0, FAST means the reset won't wait for current transmission to complete 
 
    while ((CAN_status[0] & SYNCH) == 0) 	     	//Wait for msCAN channel 0 synchronized to CAN Bus
    {	
        err_status = Check_CAN_Status(0, CAN_status);
    }
      
    err_status = Config_CAN_MB (0, 0, RXDF, 0);	//Configure the msCAN channel 0 buffer 0 to be RXDF mode (receive dataframe),"0" means to receive the specific frame with ID:0x80
    
    err_status = Check_CAN_MB_Status(0, 0, buffer_status);
    while((buffer_status[0] != NEWDATA) && (i > 0)) 			//Wait for the Receive ISR to finish and change the buffer status, NEWDATA indicates that the buffer has receive a new data
	{	
		err_status = Check_CAN_MB_Status(0, 0, buffer_status);
		i--;
	}
    datalen = Read_CAN_MB_Data(0, 0, data_rec);
    err_status = err_status;
    return datalen;
}
