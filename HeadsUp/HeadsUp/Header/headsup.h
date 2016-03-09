/* ---------- H E A D S U P . H --------- */


#ifndef __HEADSUP_H__
#define __HEADSUP_H__


#include <util/delay.h>


#define F_CPU	1000000				// CPU Freq = 1 MHz


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