#include "bat.h"


bat::bat(){
	
	ADMUX |= (1 << REFS1) | (1 << MUX0);	// MUX0 picks ADC1 Pin
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);	// Enable ADC prescaler  to 64 (125KHz @ 8 MHz)
	DIDR0 |= (1 << ADC1D);	// Disable digital input buffer on ADC1 pin
	
}


void bat::ENABLE(){
	
	ADCSRA |= ( 1 << ADEN );
	//_delay_ms( 200 );
	ADCSRA |= ( 1 << ADSC );

}


void bat::DISABLE(){
	
	ADCSRA &= ~( 1 << ADEN );
	
}

unsigned char bat::get(){
	
	adc_out adoCurrent;
	uint8_t valBatteryLevel;
	uint16_t minADCpercent = 0x02DA;
	uint16_t adcdifference = 0x0125;
	
	ENABLE();
	loop_until_bit_is_clear(ADCSRA, ADSC);
	adoCurrent = ADC - minADCpercent;
	DISABLE();
	
	valBatteryLevel = (( uint32_t )adoCurrent * 100 ) / adcdifference;
	return (( valBatteryLevel / 10 ) << 4 ) | ( valBatteryLevel % 10 );
	
}