#include "but.h"


ISR( PCINT1_vect ){
	
	extern volatile uint8_t valLastPress;

	unsigned char chrPinB;
	static uint8_t valPressed{ BUT_UP };


	_delay_ms(30);
	if( valPressed == BUT_UP ){
		memcpy(( void* )&chrPinB, ( void* )0x23, 1 );
		chrPinB = (((( chrPinB & 0x01 ) << 2 ) | ( chrPinB & 0x02 ) | (( chrPinB >> 2 ) & 0x01 )) + 1 ) % 0x08;
		if( valLastPress == BUT_UP ){
			valLastPress = chrPinB;
		}
		if( valLastPress = chrPinB ){
			valPressed = BUT_DWN;
			DDRA |= 1 << PORTA4;
			PORTA |= ( 1 << PINA4);
			_delay_ms(30);
			PORTA &= ~( 1 << PINA4);
		}
	}else{
		valPressed = BUT_UP;
	}
}

