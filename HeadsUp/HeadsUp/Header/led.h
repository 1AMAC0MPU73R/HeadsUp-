#ifndef _LED_H_
#define _LED_H_


#include <avr/io.h>


#define LED_OC1B_OUTPUT			PORTD4
#define LED_OC1A_OUTPUT			PORTD5
#define LED_OC2B_OUTPUT			PORTD6
#define LED_OC2A_OUTPUT			PORTD7	

/* typedefs */


class led{

	public:
		led();

		void set( uint8_t ledLevel );
	private:
		void INIT();
		void ENABLE();
		void DISABLE();

};


#endif /* _LED_H_ */