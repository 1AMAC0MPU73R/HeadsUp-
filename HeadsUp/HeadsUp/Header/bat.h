/*
 * bat.h
 *
 * Created: 4/12/2016 7:48:10 PM
 *  Author: Joan
 */ 


#ifndef _BAT_H_
#define _BAT_H_


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#define BAT_ADC_INPUT		PINB3


typedef uint16_t adc_out;


class bat{
	
	public:	
		bat();
		
		unsigned char get();
		
	private:
		void INIT();
		void ENABLE();
		void DISABLE();
		
};


#endif /* BATTERYMANAGEMENT_H_ */