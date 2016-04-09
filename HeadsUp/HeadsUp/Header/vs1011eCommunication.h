/*
 * vs1011eCommunication.h
 *
 * Created: 4/8/2016 6:24:34 PM
 *  Author: Jack
 */ 


#ifndef VS1011ECOMMUNICATION_H_
#define VS1011ECOMMUNICATION_H_

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

/* Register Definitions */
#define VS_PORT_DDR		DDRD
#define VS_PORT			PORTD

#define VS_RESET		PIND3
#define VS_DREQ			PIND2	// Input
#define VS_DCS			PIND1	// BSYNC
#define VS_CS			PIND0

/* DREQ Address for the VS1011e */
#define DREQ_ADDR	0x40C4
/* Definition for communication case 1 */
#define SCI_MODE_INIT_VAL	SM_SDINEW

/* Micro Controller Definitions and Initialization */
typedef unsigned char u_int8;
typedef unsigned short u_int16;

/* Function Prototypes */

void VS1011setDataDirection(void);

void WriteSci(u_int8 addr, u_int16 data);		// Example in document
u_int16 ReadSci(u_int8 addr);					// Example in document
int WriteSdi(const u_int8 *data, u_int16 bytes);// Example in document
void InitVS1011(void);

void vs1011_nulls (uint16_t nNULL);
void vs1011_sinewave_beep (uint8_t count);
void vs1011_send_data (uint8_t data);

/* Some SCI registers */
#define SCI_MODE		0
#define SCI_WRAM		6
#define SCI_WRAMADDR	7

/* Some SCI_MODE bits */
#define SM_DIFF         0	// Differential
#define SM_SETTOZERO1   1	// Set to zero
#define SM_RESET        2	// Soft Reset
#define SM_OUTOFWAV     3	// Jump out of WAV decoding
#define SM_SETTOZERO2   4	// Set to zero
#define SM_TESTS        5	// Allow SDI tests
#define SM_STREAM       6	// Stream mode
#define SM_SETTOZERO3   7	// Set to zero
#define SM_DACT         8	// DCLK active edge
#define SM_SDIORD       9	// SDI bit order
#define SM_SDISHARE_B	10
#define SM_SDINEW_B		11
#define SM_SDISHARE 	( 1 << SM_SDISHARE_B )
#define SM_SDINEW		( 1 << SM_SDINEW_B )


#endif /* VS1011ECOMMUNICATION_H_ */