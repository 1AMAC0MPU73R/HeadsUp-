/* ---------- H E A D S U P . H --------- */


#ifndef _HEADSUP_H_
#define _HEADSUP_H_


#include <avr/io.h>
#include <util/delay.h>

#include "rtc.h"
#include "lcd.h"
#include "sdcard.h"
#include "fat16.h"
#include "vs1011eCommunication.h"


#define F_CPU 8000000L		// 8 megaHz


void Test_On_PortA0();


void Test_On_PortA0(){
	
	DDRA |= 0x01;
	
	PORTA |= ( 1 << PINA0);
	_delay_ms(250);
	PORTA &= ~( 1 << PINA0);
	_delay_ms(250);
	PORTA |= ( 1 << PINA0);
}


#endif
