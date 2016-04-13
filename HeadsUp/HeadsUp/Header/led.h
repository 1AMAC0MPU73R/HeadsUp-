/*
 * led.h
 *
 * Created: 4/13/2016 4:11:18 PM
 *  Author: Jack
 */ 


#ifndef LED_H_
#define LED_H_


#include <avr/io.h>
#include <util/delay.h>


#define LED_OC1B_OUTPUT			PORTD4
#define LED_OC1A_OUTPUT			PORTD5
#define LED_OC2B_OUTPUT			PORTD6
#define LED_OC2A_OUTPUT			PORTD7	

/* typedefs */


class led{

	public:
		led();

		void set();

	private:
		void INIT();
		void ENABLE();
		void DISABLE();

};


#endif /* LED_H_ */