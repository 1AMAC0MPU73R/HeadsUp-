#include "led.h"


led::led(){
	
	TCCR2A |= ( 1 << COM2A1 ) | ( 1 << COM2B1 ) | ( 1 << WGM22 ) | ( 1 << WGM20 );		// Fast PWM
	TCCR1A |= ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM22 ) | ( 1 << WGM20 );

}

void led::ENABLE(){

	DDRD |= ( 1 << LED_OC1B_OUTPUT ) | ( 1 << LED_OC1A_OUTPUT ) | ( 1 << LED_OC2B_OUTPUT ) | ( 1 << LED_OC2A_OUTPUT );

}

void led::DISABLE(){
	
	DDRD &= ~( ( 1 << LED_OC1B_OUTPUT ) ^ ( 1 << LED_OC1A_OUTPUT ) ^ ( 1 << LED_OC2B_OUTPUT ) ^ ( 1 << LED_OC2A_OUTPUT ) );

}

void led::set() {

	static double dutyCycle = 0;
	double led_increment = 0.1;
	double led_maxVal = 1000;
	
	TCCR1B = ( 1 << CS10 );	// Starts timer
	TCCR2B = ( 1 << CS20 );

	for(int valN = 0; valN < led_maxVal; valN ++){
		_delay_ms(1);
		dutyCycle += led_increment;
		OCR1A = ( dutyCycle/100 ) * 255;
		OCR1B = ( dutyCycle/100 ) * 255;
		OCR2A = ( dutyCycle/100 ) * 255;
		OCR2B = ( dutyCycle/100 ) * 255;
		_delay_ms(1);
	}

}