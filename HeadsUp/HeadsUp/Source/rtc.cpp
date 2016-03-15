#include <avr/io.h>
#include <util/delay.h>
#include "rtc.h"


void rtc::ds1305_spi_select( ) {

	SPCR |= ( 1 << CPHA );
	PORTB |= ( 1 << SS_PIN );

}


/*
*	This routine de-selects the device by lowering the CE
*	line. Must be done after the master transmit is complete
*/
void rtc::ds1305_spi_deselect( ){

	PORTB &= ~( 1 << SS_PIN );

}


void rtc::ds1305_Init( ){

	volatile char IOReg;

	/* Outputs: MOSI and SCK out, all others inputs */
	DDRB = ( 1 << DD_MOSI_PIN ) | ( 1 << DD_SCK_PIN ) | ( 1 << DD_SS_PIN );
	/* Enable SPI, Master, CPOL=0, CPHA=1, set clock rate fck/16 */
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 );	// POSSIBLE ERROR: PULLING DATA HIGH
	
	/* Clear the SPIF bit in SPSR */
	IOReg = SPSR;
	IOReg = SPDR;

	ds1305_spi_deselect( );
}


// This routine exchanges 1 byte with the SPI port. 
unsigned char rtc::SPI_MasterTransmit( unsigned char cData )
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
void rtc::ds1305_ReadBlock( unsigned char cAddr, unsigned char *pBuffer, unsigned int iCount ){
	ds1305_spi_select( );

	SPI_MasterTransmit( cAddr );
	while( iCount-- ){
		*pBuffer++ = SPI_MasterTransmit( 0 );
	}

	ds1305_spi_deselect();
}


/*
*  WriteBlock sends iCount bytes from the buffer pBuffer
*  to the device at iAddr + 0x80. The device uses a split
*  memory map where all write addresses are at address +0x80
*/
void rtc::ds1305_WriteBlock(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount){

	ds1305_spi_select( );

	SPI_MasterTransmit( iAddr + 0x80 );
	while( iCount-- ){
		SPI_MasterTransmit( *pBuffer++ );
	}

	ds1305_spi_deselect( );
}


ds1305_Time rtc::GetCurrentTime( ){

	ds1305_Time* dstCurrent;


	ds1305_ReadBlock( 0, ( unsigned char * )dstCurrent, sizeof( ds1305_Time ));

	return dstCurrent;
}


void rtc::SetCurrentTime( ds1305_Time* dstSetTime ){

	ds1305_WriteBlock( 0, ( unsigned char * )dstSetTime, sizeof(ds1305_Time));
}

