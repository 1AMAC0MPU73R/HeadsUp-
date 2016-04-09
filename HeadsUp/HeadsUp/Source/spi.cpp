/*
 * spi.cpp
 *
 * Created: 4/8/2016 12:20:51 PM
 *  Author: Jack
 */ 
 #include "spi.h"

 void spi_select( ) {

	 SPCR |= ( 1 << CPHA );
	 PORTB |= ( 1 << SS_PIN );

 }

 /*
 *	This routine de-selects the device by lowering the CE
 *	line. Must be done after the master transmit is complete
 */
 void spi_deselect( ){

	 PORTB &= ~( 1 << SS_PIN );

 }

 void spi_init( ){

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
 unsigned char spi_master_transmit( unsigned char cData )
 {

	 SPDR  = cData;	// Set Pointer to beginning of String
	 /* wait until Char is sent */
	 while ( ! (SPSR & ( 1 << SPIF ))){
		 ;
	 }

	 return SPDR;
 }

 unsigned char spi_master_read( void )
 {
	 /* wait until Char is received */
	 while ( ! (SPSR & ( 1 << SPIF ))){
		 ;
	 }

	 return SPDR;
 }