#include "lcd.h"


lcd::lcd(){
	
		TWI_INIT();
		init_LCD();
		_delay_ms(20);
		
}


/* Initialize I2C / TWI */
void lcd::TWI_INIT(){
	
	PRR0 &= ~(1 << PRTWI);							// Do not set this bit, pg 46 atmega
	TWCR &= ~(1 << TWIE);							// Do not set Interupt Enable for polling, pg 215
	TWBR = 2;										// MUST BE SET TO 400 kHz SCL, I used a 8 MHz internal clock
	TWSR &= ~(1 << TWPS1) | (1 << TWPS0);			// No prescalling
	
}


void lcd::TWI_CheckStatusRegister(uint8_t Status){

	if( (TWSR & 0xF8) != Status ) {					// Error check for start condition
	//	TWI_ERROR();								// Call error routine
	}

}


void lcd::TWI_ClearFlagAndEnable(){

	TWCR = (1 << TWINT) | (1 << TWEN);				// Clear Flag and Enable

}

void lcd::TWI_ClearFlagAndEnableWithAck()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);// Clear Flag and Enable
}

void lcd::TWI_STOP(){
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
}

void lcd::TWI_StartCommunication(){
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	while ( ! (TWCR & (1 << TWINT)));				// wait for flag to be set
	TWI_CheckStatusRegister(TW_START);
	
}

void lcd::TWI_TransmitData(uint8_t TwiData, uint8_t StatusCode){
	
	TWDR = TwiData;
	/*After TWDR is loaded with SLA+W, a specific val must be written to TWCR, instucting the TWINT hardware
	to transmit the SLA+W present in TWDR*/
	TWI_ClearFlagAndEnable();
	while ( ! (TWCR & (1 << TWINT)));	// wait for flag to be set
	TWI_CheckStatusRegister(StatusCode);	// works!!

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
