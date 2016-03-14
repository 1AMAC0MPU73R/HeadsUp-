/********************************************************************************************************
* File Name:	Main - I2C for the LCD Screen
* Version:		1.0
* Description:	Using I2C/TWI on the ATmega MCU to control the
*				MCU on the LCD Screen
* Authors:		HeadsUp! - Jack DiSalvatore
* Date Created:	2.5.16
*
*	About TWI with ATMEGA:
*		APP(START) 	-	1. App writes to TWCR to initiate trans of START
*		HW (TWINT)	-	2. TWINT is set.  The status code inside indicates if START condition is set
*		APP(SLA+W) 	-	3. Check status of TWSR to see if START was sent
*		   		   		App loads SLA+W into TWDR, and loads appropriate control singals into TWCR,
*		   		   		making sure that TWINT is written to 1, and TWSTA is written to 0
*		HW(TWINT) 	-	4. TWINT set, status code indicates SLA+W sent, ack recv'd		
*		APP(DATA) 	-	5. Check TWSR to see if SLA+W was sent and ACK recv'd
*		   		   			App loads data into TWDR, and loads apropriate control singals into TWCR,
*				   			making sure that TWINT is written to 1
*		HW(TWINT)	-	6. TWINT set. Status code indicates data sent, ACK recv'd
*		APP(STOP) 	-	7. Check TWSR to see if data was sent and ACK recv'd
*						   APP loads appropriate control signals to send STOP into TWCR, making sure 
*						   that TWINT is written to 1
*
*	ABOUT LCD SCREEN:
*
*	START | SlaveAddress | R/W | Ack | Co=1, RS , ControlByte | Ack | DataByte | Ack| Co=0 , RS, CtrlByte
*
*	Slave Address = 0111110(R/W)	ControlByte = Co,RS,00000 	DataByte = D7,D6,D5,D4,D3,D2,D1,D0
*
*********************************************************************************************************
*/
#include "C0216CiZ.h"

unsigned char text1[]={"NEWHAVEN Display"};
unsigned char text2[]={"2x16 LCD Module "};
unsigned char text3[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char text4[]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};

const char Comsend = 0x00;
const char Datasend = 0x40;
const char Line2 = 0xC0;


// Function prototypes

void TWI_ERROR(void)
{
	PORTD |= (1 << PIND2);		// toggle PIN 2, RED LED
	_delay_ms(500);
	PORTD &= ~(1 << PIND2);
	_delay_ms(500);
}

void TWI_PassedStatusCheck(void)
{
	PORTD |= (1 << PIND3);		// toggle PIN 3, Green LED
	_delay_ms(500);
	PORTD &= ~(1 << PIND3);
	_delay_ms(500);
}

void TWI_INIT()
{
	// Initialize I2C / TWI
	PRR0 &= ~(1 << PRTWI);							// Do not set this bit, pg 46 atmega
	TWCR &= ~(1 << TWIE);							// Do not set Interupt Enable for polling, pg 215
	TWBR = 2;										// MUST BE SET TO 400 kHz SCL, I used a 8 MHz internal clock
	TWSR &= ~(1 << TWPS1) | (1 << TWPS0);			// No prescalling
}

void TWI_CheckStatusRegister(uint8_t Status)
{
	if( (TWSR & 0xF8) != Status ) {					// Error check for start condition
	//	TWI_ERROR();								// Call error routine
	}
}

void TWI_ClearFlagAndEnable()
{
	TWCR = (1 << TWINT) | (1 << TWEN);				// Clear Flag and Enable
}

void TWI_ClearFlagAndEnableWithAck()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);// Clear Flag and Enable
}

void TWI_STOP()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void TWI_StartCommunication()
{
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	while ( ! (TWCR & (1 << TWINT)));				// wait for flag to be set
	TWI_CheckStatusRegister(TW_START);
}

void TWI_TransmitData(uint8_t TwiData, uint8_t StatusCode)
{
	TWDR = TwiData;
	/*After TWDR is loaded with SLA+W, a specific val must be written to TWCR, instucting the TWINT hardware
	to transmit the SLA+W present in TWDR*/
	TWI_ClearFlagAndEnable();
	while ( ! (TWCR & (1 << TWINT)));				// wait for flag to be set
	TWI_CheckStatusRegister(StatusCode);	// works!!
}

/*****************************************************/
void Show(unsigned char *text)
{
	int n, d;
	d=0x00;
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Datasend,TW_MT_DATA_ACK);
	for(n=0;n<16;n++){
		TWI_TransmitData(*text,TW_MT_DATA_ACK);
		++text;
		}
	TWI_STOP();
}
/*****************************************************/
void nextline(void)
{
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Comsend,TW_MT_DATA_ACK);
	TWI_TransmitData(Line2,TW_MT_DATA_ACK);
	TWI_STOP();
}

void CGRAM(void)
{
 	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Comsend,TW_MT_DATA_ACK);;
	TWI_TransmitData(0x38,TW_MT_DATA_ACK);		//go to instructino table 0
	TWI_TransmitData(0x40,TW_MT_DATA_ACK);		//Set CGRAM address to 0x00
	TWI_STOP();
	_delay_ms(10);
	
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Comsend,TW_MT_DATA_ACK);
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

void init_LCD()
{
	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Comsend,TW_MT_DATA_ACK);
	TWI_TransmitData(0x38,TW_MT_DATA_ACK);						// Function Set: 8 bit bus mode, 2-line mode, single height font
	_delay_ms(100);
	TWI_TransmitData(0x39,TW_MT_DATA_ACK);						// Use Extended instruction set
	_delay_ms(100);
	TWI_TransmitData(0x14,TW_MT_DATA_ACK);						// Set Internal Osc Freq
	TWI_TransmitData(0x78,TW_MT_DATA_ACK);						// Contrast Set
	TWI_TransmitData(0x5E,TW_MT_DATA_ACK);						// Power/Icon/Contrast Control
	TWI_TransmitData(0x6D,TW_MT_DATA_ACK);						// Follower Control
	TWI_TransmitData(0x0C,TW_MT_DATA_ACK);						// Display On/Off
	TWI_TransmitData(0x01,TW_MT_DATA_ACK);						// Clear Display
	TWI_TransmitData(0x06,TW_MT_DATA_ACK);						// Entry Mode Set: Cursor/Shift as characters input
	_delay_ms(100);
	TWI_STOP();

	CGRAM();			//define CGRAM

	TWI_StartCommunication();
	TWI_TransmitData(SLA,TW_MT_SLA_ACK);
	TWI_TransmitData(Comsend,TW_MT_DATA_ACK);
	TWI_TransmitData(0x39,TW_MT_DATA_ACK);
	TWI_TransmitData(0x01,TW_MT_DATA_ACK);
	TWI_STOP();
	_delay_ms(100);
}

int test_LCD_Screen(void)
{
//	DDRD = 0b00001100;								// set pins 2 and 3 as output for LED checkin
	//PORTD = 0b00000100;							// turn number 2 off, number 3 on 

	_delay_ms(1000);								// 1 second delay

	// START PROGRAM
	TWI_INIT();

	init_LCD();
	_delay_ms(20);
	Show(text1);
	nextline();
	Show(text2);
	_delay_ms(2500);

//	init_LCD();
//	_delay_ms(20);
//	Show(text3);
//	nextline();
//	Show(text4);
//	_delay_ms(3000);

	return 1;
}