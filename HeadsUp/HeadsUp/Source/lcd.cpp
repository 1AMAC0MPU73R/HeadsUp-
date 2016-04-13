#include "lcd.h"


lcd::lcd(){
	
		TWI_INIT();
		INIT();
		_delay_ms(20);
		SHOW_WELCOME();

}


/* Initialize I2C / TWI */
void lcd::TWI_INIT(){
	
	PRR0 &= ~( 1 << PRTWI );							// Do not set this bit, pg 46 atmega
	TWCR &= ~( 1 << TWIE );							// Do not set Interupt Enable for polling, pg 215
	TWBR = 2;										// MUST BE SET TO 400 kHz SCL, I used a 8 MHz internal clock
	TWSR &= ~( 1 << TWPS1 ) | ( 1 << TWPS0 );			// No prescalling
	
}


void lcd::TWI_CHECK_STATUS( uint8_t Status ){

	if(( TWSR & 0xF8 ) != Status ){					// Error check for start condition
	//	TWI_ERROR();								// Call error routine
	}

}


void lcd::TWI_CLEAR_ENABLE(){

	TWCR = (1 << TWINT) | (1 << TWEN);				// Clear Flag and Enable

}

void lcd::TWI_CLEAR_ENABLE_ACK()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);// Clear Flag and Enable
}

void lcd::TWI_STOP(){
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
}

void lcd::TWI_START(){
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	while ( ! (TWCR & (1 << TWINT)));				// wait for flag to be set
	TWI_CHECK_STATUS(LCD_TW_START);
	
}

void lcd::TWI_TRANSMIT(uint8_t TwiData, uint8_t StatusCode){
	
	TWDR = TwiData;
	/*After TWDR is loaded with LCD_SLA+W, a specific val must be written to TWCR, instucting the TWINT hardware
	to transmit the LCD_SLA+W present in TWDR*/
	TWI_CLEAR_ENABLE();
	while ( ! (TWCR & (1 << TWINT)));	// wait for flag to be set
	TWI_CHECK_STATUS(StatusCode);	// works!!

}


void lcd::SHOW(unsigned char* text){
	
	TWI_START();
	
	TWI_TRANSMIT( LCD_SLA, LCD_TW_MT_SLA_ACK );
	TWI_TRANSMIT( LCD_SEND_DATA, LCD_TW_MT_DATA_ACK );
	for( int valN = 0; valN < LCD_LINE_SIZE; valN++ ){
		TWI_TRANSMIT( *text, LCD_TW_MT_DATA_ACK );
		++text;
	}
		
	TWI_STOP();
	
}


void lcd::SELECT_LCD_LINE_1(){
	
	TWI_START();
	
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(LCD_LINE_1,LCD_TW_MT_DATA_ACK);
	
	TWI_STOP();
}


void lcd::SELECT_LCD_LINE_2(){
	
	TWI_START();
	
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(LCD_LINE_2,LCD_TW_MT_DATA_ACK);
	
	TWI_STOP();
}

void lcd::CGRAM(void)
{
 	TWI_START();
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);;
	TWI_TRANSMIT(0x38,LCD_TW_MT_DATA_ACK);		//go to instructino table 0
	TWI_TRANSMIT(0x40,LCD_TW_MT_DATA_ACK);		//Set CGRAM address to 0x00
	TWI_STOP();
	_delay_ms(10);
	
	TWI_START();
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x00,LCD_TW_MT_DATA_ACK);		//write to first CGRAM address
	TWI_TRANSMIT(0x1E,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x18,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x14,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x12,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x01,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x00,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x00,LCD_TW_MT_DATA_ACK);		//8 bytes per character
	//continue writing to remaining CGRAM if desired
	TWI_STOP();
}

void lcd::INIT()
{
	TWI_START();
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x38,LCD_TW_MT_DATA_ACK);						// Function Set: 8 bit bus mode, 2-line mode, single height font
	_delay_ms(100);
	TWI_TRANSMIT(0x39,LCD_TW_MT_DATA_ACK);						// Use Extended instruction set
	_delay_ms(100);
	TWI_TRANSMIT(0x14,LCD_TW_MT_DATA_ACK);						// Set Internal Osc Freq
	TWI_TRANSMIT(0x78,LCD_TW_MT_DATA_ACK);						// Contrast Set
	TWI_TRANSMIT(0x5D,LCD_TW_MT_DATA_ACK);						// Power/Icon/Contrast Control
	TWI_TRANSMIT(0x6D,LCD_TW_MT_DATA_ACK);						// Follower Control
	TWI_TRANSMIT(0x0C,LCD_TW_MT_DATA_ACK);						// Display On/Off
	TWI_TRANSMIT(0x01,LCD_TW_MT_DATA_ACK);						// Clear Display
	TWI_TRANSMIT(0x06,LCD_TW_MT_DATA_ACK);						// Entry Mode Set: Cursor/Shift as characters input
	_delay_ms(100);
	TWI_STOP();

	CGRAM();			//define CGRAM

	TWI_START();
	TWI_TRANSMIT(LCD_SLA,LCD_TW_MT_SLA_ACK);
	TWI_TRANSMIT(LCD_SEND_COMMAND,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x39,LCD_TW_MT_DATA_ACK);
	TWI_TRANSMIT(0x01,LCD_TW_MT_DATA_ACK);
	TWI_STOP();
	_delay_ms(100);
}

void lcd::SHOW_WELCOME(){
	
	unsigned char cucWELCOME_TEXT_1[ LCD_LINE_SIZE ]{ ' ', '-', '-', ' ', 'H', 'e', 'a', 'd', 's', 'U', 'p', '!', ' ', '-', '-', ' ' };
	unsigned char cucWELCOME_TEXT_2[ LCD_LINE_SIZE ]{ 'S', 'l', 'e', 'e', 'p', ' ', 'E', 'n', 'v', 'i', 'r', 'o', 'm', 'e', 'n', 't' };
	
		
	print( cucWELCOME_TEXT_1, LCD_LINE_TOP );
	print( cucWELCOME_TEXT_2, LCD_LINE_BOTTOM );
	_delay_ms( 1000 );

}


void lcd::print(unsigned char* text, uint8_t valLine){
		
	if( valLine == LCD_LINE_TOP ){
		SELECT_LCD_LINE_1();
		_delay_ms(10);
		SHOW(text);
	}else if( valLine == LCD_LINE_BOTTOM ){
		SELECT_LCD_LINE_2();
		_delay_ms(10);
		SHOW(text);
	}
		
}
