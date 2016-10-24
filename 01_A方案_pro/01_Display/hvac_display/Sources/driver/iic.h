/*
 * iic.h
 *
 *  Created on: Sep 15, 2015
 *      Author: Hill
 */

#ifndef IIC_H_
#define IIC_H_

#include "derivative.h" /* include peripheral declarations */

#define I2C_WAIT_STATUS_TIMEOUT 		200000
#define I2C_ERROR_NULL                  0x00			/*!< I2C sucess*/
#define I2C_ERROR_NO_WAIT_TCF_FLAG      0x01			/*!< I2C wait TCF overtime*/
#define I2C_ERROR_NO_WAIT_IICIF_FLAG    0x02			/*!< I2C wait IICIF overtime */
#define I2C_ERROR_NO_GET_ACK            0x04			/*!< I2C no get ACK */
#define I2C_ERROR_START_NO_BUSY_FLAG    0x10			/*!< I2C fail to send start signals */
#define I2C_ERROR_STOP_BUSY_FLAG        0x20			/*!< I2C fail to send stop signal */
#define I2C_ERROR_BUS_BUSY              0x80			/*!< I2C bus busy error */

#define I2C_SEND_ACK                    0						/*!< I2C send ACK */
#define I2C_SEND_NACK                   1

void IIC_Init(void);
uint8 IIC_Start(void);
uint8 IIC_Stop(void);
uint8 IIC_WriteOneByte(uint8 dat);
uint8 IIC_ReadOneByte(uint8_t u8Ack);


#endif /* IIC_H_ */
