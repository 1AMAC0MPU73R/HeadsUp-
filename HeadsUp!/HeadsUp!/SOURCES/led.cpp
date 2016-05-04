#include "led.h"


led::led(){
	
	TCCR2A |= ( 1 << COM2A1 ) | ( 1 << COM2B1 ) | ( 1 << WGM22 ) | ( 1 << WGM20 );		// Fast PWM
	TCCR1A |= ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM22 ) | ( 1 << WGM20 );
	ENABLE();
	set( 0 );

}

void led::ENABLE(){

	DDRD |= ( 1 << LED_OC1B_OUTPUT ) | ( 1 << LED_OC1A_OUTPUT ) | ( 1 << LED_OC2B_OUTPUT ) | ( 1 << LED_OC2A_OUTPUT );
	TCCR1B = ( 1 << CS10 );	// Starts timer
	TCCR2B = ( 1 << CS20 );
}

void led::DISABLE(){
	
	set( 0 );

}

void led::set( uint8_t ledLevel ) {
	
	ledLevel = ( ledLevel < 100 ) ? ledLevel : 100;

	OCR1A = ( static_cast<double>( ledLevel ) / 100 ) * 255;
	OCR1B = ( static_cast<double>( ledLevel ) / 100 ) * 255;
	OCR2A = ( static_cast<double>( ledLevel ) / 100 ) * 255;
	OCR2B = ( static_cast<double>( ledLevel ) / 100 ) * 255;


}