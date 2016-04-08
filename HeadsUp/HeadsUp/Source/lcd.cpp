#include "lcd.h"
#include "twi.h"


lcd::lcd(){
	
	TWI_INIT();
	init_LCD();
	_delay_ms(20);
		
}


void lcd::Show(unsigned char* text){
	
	TWI_StartCommunication();
	
	TWI_TransmitData( SLA, TW_MT_SLA_ACK );
	TWI_TransmitData( SEND_DATA, TW_MT_DATA_ACK );
	for( int valN = 0; valN < LINE_SIZE; valN++ ){
		TWI_TransmitData( *text, TW_MT_DATA_ACK );
		++text;
	}
		
	TWI_STOP();
	
}


void lcd::SelectLine1(void){
	
	TWI_StartCommunication();
	
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);
	TWI_TransmitData(LINE_1,TW_MT_DATA_ACK);
	
	TWI_STOP();
}


void lcd::SelectLine2(void){
	
	TWI_StartCommunication();
	
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);
	TWI_TransmitData(LINE_2,TW_MT_DATA_ACK);
	
	TWI_STOP();
}

void lcd::CGRAM(void)
{
 	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);;
	TWI_TransmitData(0x38,TW_MT_DATA_ACK);		//go to instructino table 0
	TWI_TransmitData(0x40,TW_MT_DATA_ACK);		//Set CGRAM address to 0x00
	TWI_STOP();
	_delay_ms(10);
	
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);
	TWI_TransmitData(0x00,TW_MT_DATA_ACK);		//write to first CGRAM address
	TWI_TransmitData(0x1E,TW_MT_DATA_ACK);
	TWI_TransmitData(0x18,TW_MT_DATA_ACK);
	TWI_TransmitData(0x14,TW_MT_DATA_ACK);
	TWI_TransmitData(0x12,TW_MT_DATA_ACK);
	TWI_TransmitData(0x01,TW_MT_DATA_ACK);
	TWI_TransmitData(0x00,TW_MT_DATA_ACK);
	TWI_TransmitData(0x00,TW_MT_DATA_ACK);		//8 bytes per character
	//continue writing to remaining CGRAM if desired
	TWI_STOP();
}

void lcd::init_LCD()
{
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);
	TWI_TransmitData(0x38,TW_MT_DATA_ACK);						// Function Set: 8 bit bus mode, 2-line mode, single height font
	_delay_ms(100);
	TWI_TransmitData(0x39,TW_MT_DATA_ACK);						// Use Extended instruction set
	_delay_ms(100);
	TWI_TransmitData(0x14,TW_MT_DATA_ACK);						// Set Internal Osc Freq
	TWI_TransmitData(0x78,TW_MT_DATA_ACK);						// Contrast Set
	TWI_TransmitData(0x5D,TW_MT_DATA_ACK);						// Power/Icon/Contrast Control
	TWI_TransmitData(0x6D,TW_MT_DATA_ACK);						// Follower Control
	TWI_TransmitData(0x0C,TW_MT_DATA_ACK);						// Display On/Off
	TWI_TransmitData(0x01,TW_MT_DATA_ACK);						// Clear Display
	TWI_TransmitData(0x06,TW_MT_DATA_ACK);						// Entry Mode Set: Cursor/Shift as characters input
	_delay_ms(100);
	TWI_STOP();

	CGRAM();			//define CGRAM

	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(SEND_COMMAND,TW_MT_DATA_ACK);
	TWI_TransmitData(0x39,TW_MT_DATA_ACK);
	TWI_TransmitData(0x01,TW_MT_DATA_ACK);
	TWI_STOP();
	_delay_ms(100);
}

void lcd::test_LCD_Screen(){
	
	unsigned char text1[]={"-! TEST LINE 1 !"};
	unsigned char text2[]={"! TEST LINE 2 !-"};

	
	print(text1, LINE_TOP);
	print(text2, LINE_BOTTOM);
	_delay_ms(250);
	print(text1, LINE_BOTTOM);
	print(text2, LINE_TOP);
	_delay_ms(250);
	print(text1, LINE_TOP);
	print(text2, LINE_BOTTOM);

}


void lcd::print(unsigned char* text, uint8_t valLine){
		
	if( valLine == LINE_TOP ){
		SelectLine1();
		_delay_ms(10);
		Show(text);
	}else if( valLine == LINE_BOTTOM ){
		SelectLine2();
		_delay_ms(10);
		Show(text);
	}
		
}
