/*
 * twi.cpp
 *
 * Created: 4/8/2016 12:02:59 PM
 *  Author: Jack
 */ 
 #include "twi.h"

 /* Initialize I2C / TWI */
void TWI_INIT(){
	
	PRR0 &= ~(1 << PRTWI);							// Do not set this bit, pg 46 atmega
	TWCR &= ~(1 << TWIE);							// Do not set Interupt Enable for polling, pg 215
	TWBR = 2;										// MUST BE SET TO 400 kHz SCL, I used a 8 MHz internal clock
	TWSR &= ~(1 << TWPS1) | (1 << TWPS0);			// No prescalling
	
}


void TWI_CheckStatusRegister(uint8_t Status){

	if( (TWSR & 0xF8) != Status ) {					// Error check for start condition
	//	TWI_ERROR();								// Call error routine
	}

}


void TWI_ClearFlagAndEnable(){

	TWCR = (1 << TWINT) | (1 << TWEN);				// Clear Flag and Enable

}

void TWI_ClearFlagAndEnableWithAck()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);// Clear Flag and Enable
}

void TWI_STOP(){
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
}

void TWI_StartCommunication(){
	
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	while ( ! (TWCR & (1 << TWINT)));				// wait for flag to be set
	TWI_CheckStatusRegister(TW_START);
	
}

void TWI_TransmitData(uint8_t TwiData, uint8_t StatusCode){
	
	TWDR = TwiData;
	/*After TWDR is loaded with SLA+W, a specific val must be written to TWCR, instucting the TWINT hardware
	to transmit the SLA+W present in TWDR*/
	TWI_ClearFlagAndEnable();
	while ( ! (TWCR & (1 << TWINT)));	// wait for flag to be set
	TWI_CheckStatusRegister(StatusCode);	// works!!

}