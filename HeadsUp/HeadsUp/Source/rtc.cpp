#include <avr/io.h>
#include <util/delay.h>
#include "rtc.h"


rtc::rtc(){
	
	uint8_t tc = 0;
	uint8_t tc2 = 0xFF;


	init();
	write_block(0x0F,&tc,1);	// Enable timer and unwrite protect memory
	write_block(0x82,&tc2,1);	// set 12hr format
	
}


void rtc::spi_select( ) {

	SPCR |= ( 1 << CPHA );
	PORTB |= ( 1 << SS_PIN );

}


/*
*	This routine de-selects the device by lowering the CE
*	line. Must be done after the master transmit is complete
*/
void rtc::spi_deselect( ){

	PORTB &= ~( 1 << SS_PIN );

}


void rtc::init( ){

	char IOReg;

	/* Outputs: MOSI and SCK out, all others inputs */
	DDRB = ( 1 << DD_MOSI_PIN ) | ( 1 << DD_SCK_PIN ) | ( 1 << DD_SS_PIN );
	/* Enable SPI, Master, CPOL=0, CPHA=1, set clock rate fck/16 */
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 );	// POSSIBLE ERROR: PULLING DATA HIGH
	
	/* Clear the SPIF bit in SPSR */
	IOReg = SPSR;
	IOReg = SPDR;

	spi_deselect( );
}


// This routine exchanges 1 byte with the SPI port. 
unsigned char rtc::spi_master_transmit( unsigned char cData )
{

	SPDR  = cData;	// Set Pointer to beginning of String
	/* wait until Char is sent */
	while ( ! (SPSR & ( 1 << SPIF ))){
		;	
	}

	return SPDR;
}


/*
*	ReadBlock reads iCount bytes starting at cAddr into the
*	buffer pBuffer
*/
void rtc::read_block( unsigned char cAddr, unsigned char *pBuffer, unsigned int iCount ){
	
	spi_select( );

	spi_master_transmit( cAddr );
	while( iCount-- ){
		*pBuffer++ = spi_master_transmit( 0 );
	}

	spi_deselect();

}


/*
*  WriteBlock sends iCount bytes from the buffer pBuffer
*  to the device at iAddr + 0x80. The device uses a split
*  memory map where all write addresses are at address +0x80
*/
void rtc::write_block(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount){

	spi_select( );

	spi_master_transmit( iAddr + 0x80 );
	while( iCount-- ){
		spi_master_transmit( *pBuffer++ );
	}

	spi_deselect( );
}


rtc_time rtc::get_time( ){

	rtc_time rtmToConvert;


	read_block( 0, ( unsigned char * )&rtmToConvert, sizeof( rtc_time ));

	return rtmToConvert;
}


void rtc::set_time( rtc_time* rtmSetTime ){

	write_block( 0, ( unsigned char * )rtmSetTime, sizeof(rtc_time));
}


void rtc::rtm_to_char( rtc_time rtmToConvert, unsigned char* uchToReturn, unsigned int valCharLen ){
	
	unsigned char uchTimeOut[ valCharLen ];
	uint8_t valTimeLength = 8;
	unsigned int valTextOffset = ( valCharLen - valTimeLength ) / 2;
	uint16_t valSecs = 0x2020;
	uint16_t valMins = 0x20;
	uint16_t valHours = 0x20;
	
	
	for(unsigned int valN = 0; valN < valCharLen; valN++ ){
		uchTimeOut[valN] = ' ';
	}

	valSecs &= 0x0000;
	valSecs |= (( rtmToConvert.ucSeconds & 0xF0 ) + 0x0300 ) << 4;
	valSecs |= ( rtmToConvert.ucSeconds & 0x0F) + 0x0030;
	valMins &= 0x0000;
	valMins |= (( rtmToConvert.ucMinutes & 0xF0 ) + 0x0300 ) << 4;
	valMins |= ( rtmToConvert.ucMinutes & 0x0F) + 0x0030;
	valHours &= 0x0000;
	valHours |= (( rtmToConvert.ucHours & 0x10 ) + 0x0300 ) << 4;
	valHours |= ( rtmToConvert.ucHours & 0x0F) + 0x0030;
	uchTimeOut[valTextOffset] = valHours >> 8;
	uchTimeOut[valTextOffset + 1] = valHours & 0x00FF;
	uchTimeOut[valTextOffset + 2] = ':';
	uchTimeOut[valTextOffset + 3] = valMins >> 8;
	uchTimeOut[valTextOffset + 4] = valMins & 0x00FF;
	uchTimeOut[valTextOffset + 5] = ':';
	uchTimeOut[valTextOffset + 6] = valSecs >> 8;
	uchTimeOut[valTextOffset + 7] = valSecs & 0x00FF;
	
	memcpy( uchToReturn, uchTimeOut, valCharLen );

}

