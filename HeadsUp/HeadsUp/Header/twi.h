/*
 * twi.h
 *
 * Created: 4/8/2016 12:02:17 PM
 *  Author: Jack
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <util/delay.h>

#define TW_START			0x08	/* Master */
#define TW_REP_START		0x10
#define TW_MT_SLA_ACK		0x18	/* Master Transmitter */
#define TW_MT_SLA_NACK		0x20
#define TW_MT_DATA_ACK		0x28
#define TW_MT_DATA_NACK		0x30
#define TW_MT_ARB_LOST		0x38
#define TW_MR_ARB_LOST		0x38	/* Master Receiver */
#define TW_MR_SLA_ACK		0x40
#define TW_MR_SLA_NACK		0x48
#define TW_MR_DATA_ACK		0x50
#define TW_MR_DATA_NAC		0x58
#define	SLA					0x7C	// LCD will always be in WRITE mode, there is no READ mode. USE 0x7C

/* Function Prototypes */
void TWI_INIT();
void TWI_CheckStatusRegister(uint8_t Status);
void TWI_ClearFlagAndEnable();
void TWI_ClearFlagAndEnableWithAck();
void TWI_STOP();
void TWI_StartCommunication();
void TWI_TransmitData(uint8_t TwiData, uint8_t StatusCode);


#endif /* TWI_H_ */