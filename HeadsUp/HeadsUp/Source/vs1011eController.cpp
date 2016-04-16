/*	This File Handles the Comunication between the VS1011e and the micro controller
*	It utilizes the SPI protocol
*/
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "vs1011eController.h"
#include "spi.h"

void vs1011_hardreset (void){

    asm volatile ("cli");	// disable interrupts because DREQ is an interrupt trigger

    VS_PORT &= ~(1 << VS_RESET);	// put VS1011 in reset state
    _delay_us (1);
    VS_PORT |= (1 << VS_RESET);	// put VS1011 in normal state

/*
    vs1011_nulls (2048);	// send somes NULLs
    VS_PORT &= ~_BV (VS_CS);	// select VS1011 via Chip Select
    vs1011_write (VS1011_MODE, _BV (SM_RESET));	// trigger soft reset
    _delay_us (10);		// an wait
*/

    while( ! (VS_DREQ_INP & (1 << VS_DREQ) ) );	// wait for DREQ changes back to high

    vs1011_nulls(32);		// send somes NULLs
    WriteSci(VS1011_CLOCKF, VS1011_CLOCKFVALUE);	// write compensation value

    VS_PORT |= (1 << VS_CS);	// deselect VS1011 via Chip Select

    asm volatile ("sei");	// disable interrupts because DREQ is an interrupt trigger
}


void vs1011_softreset (void){

    asm volatile ("cli");	// disable interrupts because DREQ is an interrupt trigger

    vs1011_nulls(2048);	// send somes NULLs

    VS_PORT &= ~(1 << VS_CS);	// select VS1011 via Chip Select

    WriteSci ( VS1011_MODE, (1 << SM_RESET) );	// trigger soft reset

    _delay_us (10);		// an wait

    while ( ! (VS_DREQ_INP & (1 << VS_DREQ) ) );	// wait for DREQ changes back to high

    VS_PORT |= (1 << VS_CS);	// deselect VS1011 via Chip Select

    asm volatile ("sei");	// disable interrupts because DREQ is an interrupt trigger
}
//*****************************************************************************
// Function: vs1011_setvolume
// Parameters: Volume value
//             Balance value
// Returns: none.
//
// Description: Set audio volume. Create a balance effect by adding and
//              subtracting the balance value from absolute volume value.
//*****************************************************************************
void vs1011_setvolume (uint8_t volume, uint8_t balance){

    uint16_t tempdata;

    int8_t tempbal = (int8_t) (100 - balance);

    // set volume left channel
    tempbal = -tempbal;
    tempdata = volume;
    if (tempbal > 0)
        tempdata += tempbal;

    tempdata <<= 8;
    // set volume right channel
    tempdata += volume;
    tempbal = -tempbal;
    if (tempbal > 0)
        tempdata += tempbal;

    VS_PORT &= ~(1 << VS_CS);	// select VS1011 via Chip Select

    WriteSci (VS1011_VOL, tempdata);	// set VS1011 volume register

    WriteSci (VS1011_MODE, (1 << SM_DIFF) );  // enable differential mode

    VS_PORT |= (1 << VS_CS);	// deselect VS1011 via Chip Select
}

//*****************************************************************************
// Function: vs1011_setbassboost
// Parameters: Amplitude value
//             Freqency limit value
// Returns: none.
//
// Description: Configure the VS1011 bass boost algorithm with specific amplitude
//              and frequency limit.
//*****************************************************************************
extern void vs1011_setbassboost (uint8_t amp_freq){

    uint16_t tempdata = 0;

    tempdata += amp_freq;

    WriteSci(VS1011_BASS, tempdata);	// set bass enhancer register
}

void vs1011_nulls (uint16_t nNULL) {

	for (; nNULL > 0; nNULL--) {
		spi_master_transmit (0);
	}

}

void sinewave_beep_test (uint8_t count) {

	static uint8_t buf[8] = { 0x53, 0xEF, 0x6E, 4,	// sine on
		0x45, 0x78, 0x69, 0x74
	};				// sine off
	vs1011_nulls (8);
	asm volatile ("cli");
	WriteSci ( SCI_MODE, ( 1 << SM_TESTS ) );

	while (count--) {
		for (uint8_t i = 0; i < 4; i++) {	// 4
			spi_master_transmit (buf[i]);
			//vs1011_send_data (buf[i]);
		}
		vs1011_nulls (4);
		for (uint8_t a = 0; a < 100; a++)
		_delay_ms (1);
		
		for (uint8_t i = 4; i < 8; i++) {	// 8
			spi_master_transmit (buf[i]);
		}
		vs1011_nulls (8);
		for (uint8_t a = 0; a < 100; a++)
		_delay_ms (1);
	}
	asm volatile ("sei");

}

void vs1011_send32 (char * pData){

    for (uint8_t i = 0; i < 32; i++) {
        spi_master_transmit(*pData++);	// send 32 bytes
    }
}

/* Function for SPI transmit taken from guy onlines example code */
/*
void vs1011_send_data (uint8_t data) {

	asm volatile ("cli");	// disable interrupts

	VS_PORT |= ( 1 << VS_DCS	);	// start BSYNC signal

	SPDR = data;		// send byte
	asm volatile ("nop");	// some delay
	asm volatile ("nop");
	asm volatile ("nop");

	VS_PORT &= ~( 1 << VS_DCS );	// stop BSYNC signal

	while ( ! (SPSR & ( 1 << SPIF)));	// wait until transfer finished

	asm volatile ("sei");	// enable interrupts
}
*/

/* SCI Operations */

void WriteSci(u_int8 addr, u_int16 data) {
	while( VS_DREQ == 0 ) {
		;									// Wait until DREQ is High
	}

	VS_PORT &= ~( 1 << VS_CS );				// Active VS_CS
	spi_master_transmit( 2 );				// Write Command Code
	spi_master_transmit( addr );			// SCI Register Number
	spi_master_transmit( (u_int8)( data >> 8 ) );
	spi_master_transmit( (u_int8)(data & 0xFF) );
	VS_PORT |= ( 1 << VS_CS );				// Deactivate VS_CS
}

u_int16 ReadSci(u_int8 addr) {
	u_int16 res;

	while( VS_DREQ == 0 ) {
		;									// Wait until DREQ is High
	}

	VS_PORT &= ~( 1 << VS_CS );				// Active VS_CS
	spi_master_transmit( 3 ); // Read command code
	spi_master_transmit( addr ); // SCI register number
	res = (u_int16)spi_master_read() << 8 ;
	res |= spi_master_read();
	VS_PORT |= ( 1 << VS_CS );				// Deactive VS_CS

	return res;
}

/* SDI Operations */

int WriteSdi(const u_int8 *data, u_int8 bytes) {
	u_int8 i;

	if( bytes > 32 ) {
		return -1;							// ERROR: Too many bytes to transfer
	}

	while( VS_DREQ == 0 ) {
		;									// Wait until DREQ is High
	}

	VS_PORT &= ~( 1 << VS_DCS );			// Activate DCS

	for( i = 0; i < bytes; i++) {
		spi_master_transmit( *data++ );
	}

	VS_PORT |= ( 1 << VS_DCS );				// Deactivate DCS

	return 0;
}

/* Initialization */
void InitVS1011(void) {
	VS_PORT &= ~( 1 << VS_RESET );			// Reset Vs1011
	VS_PORT |= ( 1 << VS_CS );				// CS High
	VS_PORT |= ( 1 << VS_DCS );				// DCS High
	VS_PORT |= ( 1 << VS_RESET );			// Out of Reset
	// SCI_MODE_INT_VAL separately defined for each (7) connection case
	WriteSci( SCI_MODE, SCI_MODE_INIT_VAL );
	// If DREQ pin is not used, add a delay that is long enough
	// See Data sheet Chapter Switching "Characteristics - Boot Initialization"
	// for how long to wait
}

void VS1011setDataDirection(void) {
	/* Set Data Direction for VS_PORT */
	VS_PORT_DDR |=	0b11111011;

}