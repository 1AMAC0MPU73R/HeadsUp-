/*
 * SPI_RTC.c
 *
 * Created: 2/28/2016 12:40:22 PM
 * Author : Jack
 *
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include "ds1305.h"

/* Slave Select */
#define SS_PIN			PB4
#define DD_SS_PIN		DDB4

/* Data Out */
#define MOSI_PIN		PB5
#define DD_MOSI_PIN		DDB5

/* Data In */
#define MISO_PIN		PB6
#define DD_MISO_PIN		DDB6

/* Clock */
#define SCK_PIN			PB7
#define DD_SCK_PIN		DDB7

/* Function Prototypes */

/* Globals */


/*
*	 This routine selects the ds1305 device. Must be called
*	before transferring data to the SPI port
*
*	Need to insure that this is CPHA = 1 due to the
*	timing requirements of the part. If the SPI bus is shared
*	with other devices, their Select routines should reset these
*	parameters to their value.
*	Raising the line enables the part
*/
void ds1305_spi_select() {
	SPCR |= (1<<CPHA);
	PORTB |= (1<<SS_PIN);
}

/*
*	This routine de-selects the device by lowering the CE
*	line. Must be done after the master transmit is complete
*/
void ds1305_spi_deselect() {
	PORTB &= ~(1<<SS_PIN);
}

void ds1305_Init()
{
	volatile char IOReg;
	/* Outputs: MOSI and SCK out, all others inputs */
	DDRB = (1<<DD_MOSI_PIN) | (1<<DD_SCK_PIN) | (1<<DD_SS_PIN);
	/* Enable SPI, Master, CPOL=0, CPHA=1, set clock rate fck/16 */
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);	// POSSIBLE ERROR: PULLING DATA HIGH
	
	/* Clear the SPIF bit in SPSR */
	IOReg = SPSR;
	IOReg = SPDR;
	ds1305_spi_deselect();
}

// This routine exchanges 1 byte with the SPI port. 
unsigned char SPI_MasterTransmit(unsigned char cData)
{
	// Set Pointer to beginning of String

	SPDR  = cData;
	/* wait until Char is sent */
	while (!(SPSR & (1<<SPIF))){
		;	
	}
	return SPDR;
}

/*
*	ReadBlock reads iCount bytes starting at cAddr into the
*	buffer pBuffer
*/
void ds1305_ReadBlock(unsigned char cAddr, unsigned char *pBuffer, unsigned int iCount)
{
	ds1305_spi_select();
	SPI_MasterTransmit(cAddr);
	while(iCount--)
	{
		*pBuffer++ = SPI_MasterTransmit(0);
	}
	ds1305_spi_deselect();
}

/*
*  WriteBlock sends iCount bytes from the buffer pBuffer
*  to the device at iAddr + 0x80. The device uses a split
*  memory map where all write addresses are at address +0x80
*/
void ds1305_WriteBlock(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount)
{
	ds1305_spi_select();
	SPI_MasterTransmit(iAddr + 0x80);
	while(iCount--)
	{
		SPI_MasterTransmit(*pBuffer++);
	}
	ds1305_spi_deselect();
}
