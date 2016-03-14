/*
 * C0216CiZ.h
 *
 * Created: 3/14/2016 4:06:34 PM
 *  Author: Jack
 */ 


#ifndef C0216CIZ_H_
#define C0216CIZ_H_

/* Put the below in a header file */
// #include <compat/twi.h>		// USE THIS LATER!!!! SUPER HELPFUL

#include <avr/io.h>
#include <util/delay.h>

/* TWSR values (not bits) */
/* Master */
#define TW_START        	  0x08
#define TW_REP_START          0x10
/* Master Transmitter */
#define TW_MT_SLA_ACK         0x18
#define TW_MT_SLA_NACK        0x20
#define TW_MT_DATA_ACK        0x28
#define TW_MT_DATA_NACK       0x30
#define TW_MT_ARB_LOST        0x38
/* Master Receiver */
#define TW_MR_ARB_LOST        0x38
#define TW_MR_SLA_ACK         0x40
#define TW_MR_SLA_NACK        0x48
#define TW_MR_DATA_ACK        0x50
#define TW_MR_DATA_NACK       0x58

#define	SLA					0x7C
// LCD will always be in WRITE mode, there is no READ mode. USE 0x7C

extern int test_LCD_Screen(void);

#endif /* C0216CIZ_H_ */