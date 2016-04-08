#ifndef _LCD_H_
#define _LCD_H_


#include <avr/io.h>
#include <util/delay.h>

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

		void Show(unsigned char *text);
		void SelectLine1();
		void SelectLine2();

		void CGRAM();

};


#endif