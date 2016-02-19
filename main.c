#include <avr/io.h>
#include <util/delay.h>
#include "twi_lcd.h"

// Global Variables
unsigned char text1[]={"NEWHAVEN Display"};
unsigned char text2[]={"2x16 LCD Module "};
unsigned char text3[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char text4[]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};


int main(void)
{
	DDRD |= 0b00001100;								// Set pins 2 and 3 as output
	//PORTD |= 0b00000100;							// Turn number 2 off, number 3 on
	
	begin_LCD_twi();
	
}

// Error checking and handling
void TWI_Error(void)
{
	PORTD |= (1 << PIND2);								// Toggle PIN 2, RED LED
	_delay_ms(500);
	PORTD &= ~(1 << PIND2);
	_delay_ms(500);
}

void TWI_Passed(void)
{
	PORTD |= (1 << PIND3);								// Toggle PIN 3, Green LED
	_delay_ms(500);
	PORTD &= ~(1 << PIND3);
	_delay_ms(500);
}


void TWI_out(uint8_t TwiData, uint8_t StatusCode)
{
	TWDR = TwiData;
	TWCR = (1 << TWINT) | (1 << TWEN);					// Clear flag and enable
	while (!(TWCR & (1 << TWINT)));						// Wait for flag to be set

/* ----- BREAKS HERE: RETURNS 0x20 (NO ACKNOWLEDGE) ALL -----
             CALLS TO TWI_OUT AFTER THIS POINT FAIL
		                  SIMILARLY
------------------------------------------------------------- */
	
	if((TWSR & 0xF8) != StatusCode) {
		TWI_Error();
	} 
}

void TWI_Start()
{
	// Send Start Condition
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);	// Prepare Start condition
	while (!(TWCR & (1 << TWINT)));						// Wait for flag to be set
	if((TWSR & 0xF8) != TW_START) {
		TWI_Error();
	} 
}

void Show(unsigned char *text)
{
	int n, d;
	d=0x00;
	TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(Datasend,TW_MT_DATA_ACK);
	for(n=0;n<16;n++){
		TWI_out(*text,TW_MT_DATA_ACK);
		++text;
	}
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
}

void nextLine()
{
	TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(Comsend,TW_MT_DATA_ACK);
	TWI_out(Line2,TW_MT_DATA_ACK);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
}

void CGRAM(void)
{
	TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(Comsend,TW_MT_DATA_ACK);
	TWI_out(0x38,TW_MT_DATA_ACK);
	TWI_out(0x40,TW_MT_DATA_ACK);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
	_delay_ms(10);
	
	TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(Datasend,TW_MT_DATA_ACK);
	TWI_out(0x00,TW_MT_DATA_ACK);
	TWI_out(0x1E,TW_MT_DATA_ACK);
	TWI_out(0x18,TW_MT_DATA_ACK);
	TWI_out(0x14,TW_MT_DATA_ACK);
	TWI_out(0x12,TW_MT_DATA_ACK);
	TWI_out(0x01,TW_MT_DATA_ACK);
	TWI_out(0x00,TW_MT_DATA_ACK);
	TWI_out(0x00,TW_MT_DATA_ACK);						// 8 bytes per character
	// continue writing to remaining CGRAM if desired
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
}

void init_LCD()
{
	TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(0x38,TW_MT_DATA_ACK);
	_delay_ms(10);
	TWI_out(0x39,TW_MT_DATA_ACK);
	_delay_ms(10);
	TWI_out(0x14,TW_MT_DATA_ACK);
	TWI_out(0x78,TW_MT_DATA_ACK);
	TWI_out(0x5E,TW_MT_DATA_ACK);
	TWI_out(0x6D,TW_MT_DATA_ACK);
	TWI_out(0x0C,TW_MT_DATA_ACK);
	TWI_out(0x01,TW_MT_DATA_ACK);
	TWI_out(0x06,TW_MT_DATA_ACK);
	_delay_ms(10);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
	
	//CGRAM();											// Define CGRAM
	
	/*TWI_Start();
	TWI_out(Slave,TW_MT_SLA_ACK);
	TWI_out(Comsend,TW_MT_DATA_ACK);
	TWI_out(0x39,TW_MT_DATA_ACK);
	TWI_out(0x01,TW_MT_DATA_ACK);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// Stop Condition
	_delay_ms(10);*/
}

// Start Here
void begin_LCD_twi()
{
	// Initialize TWI Registers
	PRR0 &= ~(1 << PRTWI);							// Turn off power reduction register
	TWCR &= ~(1 << TWIE);							// Turn off interrupts for polling
	TWBR = 2;										// Set Bit Rate Register at 2 for 1 MHz CPU Clock and SCL TWI Clock at 50kHz
	TWSR &= ~(1 << TWPS1) | (1 << TWPS0);			// No Prescaler to mess up the Status Register
	
	// START PROGRAM
	_delay_ms(1000);								// 1 second delay
	
	while(1) {
		init_LCD();
		//_delay_ms(2);
		//Show(text1);
		//nextLine();
		//Show(text2);
		//_delay_ms(2500);
		
		//TWI_Passed();	
	}
