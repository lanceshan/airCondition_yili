/*
 * iic.c
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */
/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "iic.h"

void IIC_Init(void)
{
	SIM_SCGC |= SIM_SCGC_I2C1_MASK;	//enable IIC clock
	I2C1_C1 = 0x00;					//I2C1_C1 = 0b1011 0100使能IIC，不产生中断，主模式，发送，接收数据后应答，可重复启动，无唤醒功能
	I2C1_FLT = (I2C_FLT_STOPF_MASK | I2C_FLT_STARTF_MASK | I2C_FLT_FLT(0x00)); /* Clear bus status interrupt flags */
	I2C1_S1 = I2C_S_IICIF_MASK;          /* Clear interrupt flag */
	SIM_PINSEL1 &= (uint32_t)~(uint32_t)(SIM_PINSEL1_I2C1PS_MASK);	//选择PTE0,PTE1
	I2C1_C2 = I2C_C2_AD(0x00);			//从机地址为0，7位地址，波特率由主机决定
	I2C1_FLT = I2C_FLT_FLT(0x00);       /* Set glitch filter register */
//	I2C1_SMB = I2C_SMB_SLTF_MASK;
	I2C1_F = (I2C_F_MULT(0x02) | I2C_F_ICR(0x1f));	//mul = 2,ICR = 0x1f;
	I2C1_SLTL = 0x00;
	I2C1_SLTH = 0x00;
	I2C1_C1 |= I2C_C1_IICEN_MASK;
}

uint8 IIC_Start(void)
{
	uint32_t u32Timeout=0x00;
	uint8_t  u8ErrorStatus=0;

	I2C1_C1 |= I2C_C1_TX_MASK;
	I2C1_C1 |= I2C_C1_MST_MASK;

    while( !(I2C1_S1 & I2C_S_BUSY_MASK) && ( u32Timeout < I2C_WAIT_STATUS_TIMEOUT) )
	{
    	u32Timeout ++;
	}

	if( u32Timeout == I2C_WAIT_STATUS_TIMEOUT )
	{
	   u8ErrorStatus |= I2C_ERROR_START_NO_BUSY_FLAG;
	}

	return u8ErrorStatus;	
}

uint8 IIC_Stop(void)
{
    uint32_t u32Timeout = 0x00;
    uint8_t u8ErrorStatus = 0;
    
    I2C1_C1 &= ~I2C_C1_MST_MASK;
    
    while( ( (I2C1_S1 & I2C_S_BUSY_MASK) == I2C_S_BUSY_MASK ) && ( u32Timeout < I2C_WAIT_STATUS_TIMEOUT))
    {
    	u32Timeout ++;
   	}
   	if( u32Timeout == I2C_WAIT_STATUS_TIMEOUT )
   	{
   		u8ErrorStatus |= I2C_ERROR_STOP_BUSY_FLAG;
    }

    return u8ErrorStatus;
}

uint8 IIC_WriteOneByte(uint8 dat)
{

    uint32_t u32Timeout = 0x00;
    uint8_t u8ErrorStatus = 0;

    while (((I2C1_S1 & I2C_S_TCF_MASK) != I2C_S_TCF_MASK) 
    		&& (u32Timeout < I2C_WAIT_STATUS_TIMEOUT))
    {
        u32Timeout ++;
    }
    
    if (u32Timeout >= I2C_WAIT_STATUS_TIMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_TCF_FLAG; 
        return u8ErrorStatus;
    }
    
    I2C1_C1 |= I2C_C1_TX_MASK;
    I2C1_D = dat;

    u32Timeout = 0;
    while (((I2C1_S1 & I2C_S_IICIF_MASK) !=  I2C_S_IICIF_MASK)
            && (u32Timeout<I2C_WAIT_STATUS_TIMEOUT))
    {
        u32Timeout ++;
    }
    if (u32Timeout >= I2C_WAIT_STATUS_TIMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_IICIF_FLAG; 
        return u8ErrorStatus;
    }

    /* clear IICIF flag */
    I2C1_S1 |= I2C_S_IICIF_MASK;
    if (I2C1_S1 & I2C_S_RXAK_MASK) 
    {
        u8ErrorStatus |= I2C_ERROR_NO_GET_ACK;
    }
    return u8ErrorStatus;
}

uint8 IIC_ReadOneByte(uint8_t u8Ack)
{
    uint32_t u32Timeout = 0x00;
    uint8_t u8ErrorStatus = 0;
    uint8 dat=0;

    while (((I2C1_S1 & I2C_S_TCF_MASK) !=  I2C_S_TCF_MASK)
            && (u32Timeout<I2C_WAIT_STATUS_TIMEOUT))
    {
        u32Timeout ++;
    }
    if (u32Timeout >= I2C_WAIT_STATUS_TIMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_TCF_FLAG; 
        return u8ErrorStatus;
    }
    
    I2C1_C1 &= ~I2C_C1_TX_MASK;

    if( u8Ack )			/* send out nack */ 
    	I2C1_C1 |= I2C_C1_TXAK_MASK; 
    else				/* send out ack */
    	I2C1_C1 &= ~I2C_C1_TXAK_MASK;
    
    dat = I2C1_D;
    
    u32Timeout = 0;
    while (((I2C1_S1 & I2C_S_IICIF_MASK) !=  I2C_S_IICIF_MASK)
            && (u32Timeout < I2C_WAIT_STATUS_TIMEOUT))
    {
        u32Timeout ++;
    }
    if (u32Timeout >= I2C_WAIT_STATUS_TIMEOUT)
    {
        u8ErrorStatus |= I2C_ERROR_NO_WAIT_IICIF_FLAG; 
        return u8ErrorStatus;
    }
    /* clear IICIF flag */
    I2C1_S1 |= I2C_S_IICIF_MASK;
    return dat;
}
