/*
 * vs1011eCommunication.h
 *
 * Created: 4/8/2016 6:24:34 PM
 *  Author: Jack
 */ 


#ifndef VS1011ECONTROLLER_H_
#define VS1011ECONTROLLER_H_

/* Register Definitions */
#define VS_PORT_DDR		DDRD
#define VS_PORT			PORTD

#define VS_RESET		PIND3
#define VS_DREQ			PIND2	// Input
#define VS_DREQ_INP		PIND	// VS1011 DREQ Input port
#define VS_DCS			PIND1	// BSYNC
#define VS_CS			PIND0

/* DREQ Address for the VS1011e */
#define DREQ_ADDR	0x40C4
/* Definition for communication case 1 */
#define SCI_MODE_INIT_VAL	SM_SDINEW


/* Some SCI registers */
#define SCI_MODE		0
#define SCI_WRAM		6
#define SCI_WRAMADDR	7

// VS1011 registers
#define VS1011_MODE         0x00	// Mode control
#define VS1011_STATUS       0x01	// Status of VS1011b
#define VS1011_BASS         0x02	// Built-in bass enhancer
#define VS1011_CLOCKF       0x03	// Clock freq + doubler
#define VS1011_DECODE_TIME  0x04	// Decode time in seconds
#define VS1011_AUDATA       0x05	// Misc. audio data
#define VS1011_WRAM         0x06	// RAM write
#define VS1011_WRAMADDR     0x07	// Base address for RAM write
#define VS1011_HDAT0        0x08	// Stream header data 0
#define VS1011_HDAT1        0x09	// Stream header data 1
#define VS1011_AIADDR       0x0A	// Start address of application
#define VS1011_VOL          0x0B	// Volume control
#define VS1011_AICTRL0      0x0C	// Application control register 0
#define VS1011_AICTRL1      0x0D	// Application control register 1
#define VS1011_AICTRL2      0x0E	// Application control register 2
#define VS1011_AICTRL3      0x0F	// Application control register 3

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

/* Clock */
#define VS1011_XTAL	  24000000UL	// 24.000Mhz

/* For VS1011_USE_CLKDOUBLER */
#define TEMPCLOCKFVALUE  ((VS1011_XTAL/2000UL)+0x8000)

/* For non clock doubler */
//#define TEMPCLOCKFVALUE  (VS1011_XTAL/2000UL)

#define VS1011_CLOCKFVALUE  (unsigned int)(TEMPCLOCKFVALUE)

/* Micro Controller Definitions and Initialization */
typedef unsigned char u_int8;
typedef unsigned short u_int16;

/* Function Prototypes */

void VS1011setDataDirection(void);

void WriteSci(u_int8 addr, u_int16 data);		// Example in document
u_int16 ReadSci(u_int8 addr);					// Example in document
int WriteSdi(const u_int8 *data, u_int16 bytes);// Example in document

extern void InitVS1011(void);
extern void vs1011_hardreset (void);
extern void vs1011_softreset (void);

extern void vs1011_setvolume (uint8_t volume, uint8_t balance);
extern void vs1011_setbassboost (uint8_t amp_freq);

extern void vs1011_nulls (uint16_t nNULL);
void sinewave_beep_test (uint8_t count);

extern void vs1011_send_data (uint8_t data);
extern void vs1011_send32 (char * pData);


#endif /* VS1011ECONTROLLER_H_ */