#ifndef _LCD_H_
#define _LCD_H_


#include <avr/io.h>
#include <util/delay.h>


/* ------------------- LCD I2C/INTERFACE DEFINITIONS ------------------ */

#define LCD_TW_START			0x08	// Master Signals
#define LCD_TW_REP_START		0x10
#define LCD_TW_MT_SLA_ACK		0x18	//  Master Transmitter Signals
#define LCD_TW_MT_SLA_NACK		0x20
#define LCD_TW_MT_DATA_ACK		0x28
#define LCD_TW_MT_DATA_NACK		0x30
#define LCD_TW_MT_ARB_LOST		0x38
#define LCD_TW_MR_ARB_LOST		0x38	// Master Receiver Signals
#define LCD_TW_MR_SLA_ACK		0x40
#define LCD_TW_MR_SLA_NACK		0x48
#define LCD_TW_MR_DATA_ACK		0x50
#define LCD_TW_MR_DATA_NAC		0x58
#define	LCD_SLA					0x7C	// LCD slave address for I2C writing, no reading functionality
#define LCD_SEND_COMMAND		0x00	// Transmission content flags
#define LCD_SEND_DATA			0x40

/* ---------------------------------------------------------------- */

/* -------------------- LCD DISPLAY DEFINITIONS ------------------- */

#define LCD_LINE_SIZE			16		// Screen will be configured to always display 2x16 bit lines
#define LCD_LINE_1				0x80	// Line input addresses
#define LCD_LINE_2				0xC0
#define LCD_LINE_TOP			0		// Line selection flags
#define LCD_LINE_BOTTOM			1

/* ---------------------------------------------------------------- */


/* --------------------- LCD CLASS DEFINITION --------------------- */

class lcd{

	public:
		lcd();
				
		void print(unsigned char* text, uint8_t valLine);

	private:
		void INIT();
		void TWI_INIT();
		void TWI_CHECK_STATUS(uint8_t Status);
		void TWI_CLEAR_ENABLE();
		void TWI_CLEAR_ENABLE_ACK();
		void TWI_STOP();
		void TWI_START();
		void TWI_TRANSMIT(uint8_t TwiData, uint8_t StatusCode);
		void SHOW(unsigned char *text);
		void SELECT_LCD_LINE_1();
		void SELECT_LCD_LINE_2();
		void SHOW_WELCOME();

		void CGRAM();

};

/* ---------------------------------------------------------------- */


#endif