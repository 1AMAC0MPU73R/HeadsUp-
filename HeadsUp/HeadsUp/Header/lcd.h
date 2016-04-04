#ifndef _LCD_H_
#define _LCD_H_


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

#define SEND_COMMAND 0x00
#define SEND_DATA 0x40

#define LINE_SIZE 16
#define LINE_1 0x80
#define LINE_2 0xC0
#define LINE_TOP 0
#define LINE_BOTTOM 1


class lcd{

	public:
		lcd();
		
		void test_LCD_Screen();
		
		void print(unsigned char* text, uint8_t valLine);

	private:
		void init_LCD();
		void TWI_INIT();
		void TWI_CheckStatusRegister(uint8_t Status);
		void TWI_ClearFlagAndEnable();
		void TWI_ClearFlagAndEnableWithAck();
		void TWI_STOP();
		void TWI_StartCommunication();
		void TWI_TransmitData(uint8_t TwiData, uint8_t StatusCode);
		void Show(unsigned char *text);
		void SelectLine1();
		void SelectLine2();

		void CGRAM();

};


#endif