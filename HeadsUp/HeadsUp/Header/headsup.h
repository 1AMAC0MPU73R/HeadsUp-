/* ---------- H E A D S U P . H --------- */


#ifndef _HEADSUP_H_
#define _HEADSUP_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rtc.h"
#include "lcd.h"


#define F_CPU 8000000L		// 8 megaHz


rtc rtcDS;
lcd lcdNHD;
rtc_time rtmCurrent{0x50, 0x59, 0x72, 0x02, 0x11, 0x04, 0x16};


void HeadsUp_Init();
void Test_On_PortA0();
void Enable_PCINT2();
void Enable_PCINT3();
void Menu_Main();


void HeadsUp_Init(){
	
	unsigned char uchCurrentTime[ LINE_SIZE ] ;
	
	
	Test_On_PortA0();
	lcdNHD.test_LCD_Screen();
	
	rtcDS.set(&rtmCurrent);

	Enable_PCINT2();
	Enable_PCINT3();
	sei();
	
	rtcDS.rtm_to_char( rtmCurrent, uchCurrentTime, LINE_SIZE );
	lcdNHD.print( uchCurrentTime , LINE_TOP );

}


void Enable_PCINT3(){
	
	DDRA &= ~( 1 << PORTA3 );
	PORTA |= 1 << PORTA3;
	PCICR |= 1 << PCIE0;
	PCMSK0 |= 1 << PCINT3;
	PCIFR |= 1 << PCIF0;

}


void Enable_PCINT2(){
	
	DDRA &= ~( 1 << PORTA2 );
	PORTA |= 1 << PORTA2;
	PCICR |= 1 << PCIE0;
	PCMSK0 |= 1 << PCINT2;
	PCIFR |= 1 << PCIF0;

}


void Test_On_PortA0(){
	
	DDRA |= 1 << PORTA0;
	
	PORTA |= ( 1 << PINA0);
	_delay_ms(250);
	PORTA &= ~( 1 << PINA0);
	_delay_ms(250);
	PORTA |= ( 1 << PINA0);
	
}


ISR( PCINT0_vect ){	
	
	unsigned char chrPinA;
	unsigned char uchCurrentTime[ LINE_SIZE ] ;
	
	
	memcpy(( void* )&chrPinA, ( void* )0x20, 1 );
	
	if((( chrPinA >> PORTA3 ) & 0x01 ) == 0x00 ){
		DDRA |= 1 << PORTA4;	
		PORTA |= ( 1 << PINA4);
		_delay_ms(250);
		PORTA &= ~( 1 << PINA4);
		rtcDS.clear_interupt( RTC_ALARM_0 );
	}else if((( chrPinA >> PORTA2 ) & 0x01 ) == 0x00 ){
		rtmCurrent = rtcDS.get();
		rtcDS.clear_interupt( RTC_ALARM_1 );
	}
	
}


void Menu_Main(){
	
	unsigned char chrMenuTop[LINE_SIZE];
	unsigned char chrMenuBot[LINE_SIZE]{ 'A', 'L', 'R', 'M', '|', 'B', 'A', 'T', 'X', 'X', 'X', '|', 'O', 'P', 'T', 'S' };
	
	lcdNHD.print( chrMenuBot , LINE_BOTTOM );
	
	for(;;){
		rtcDS.rtm_to_char( rtmCurrent, chrMenuTop, LINE_SIZE );
		lcdNHD.print( chrMenuTop , LINE_TOP );
		_delay_ms(25);
	}
	
}

#endif
