/* ---------- H E A D S U P . H --------- */


#ifndef __HEADSUP_H__
#define __HEADSUP_H__


#include <avr/io.h>
#include <util/delay.h>
#include "ds1305.h"


#define F_CPU	1000000				// CPU Freq = 1 MHz


void Test_On_PortA0();
void Start_RTC();


void Test_On_PortA0(){
	
	DDRA |= 0x01;
	
	PORTA |= ( 1 << PINA0);
	_delay_ms(250);
	PORTA &= ~( 1 << PINA0);
	_delay_ms(250);
	PORTA |= ( 1 << PINA0);
}


void Start_RTC(){
	
	uint8_t tc = 0;


	ds1305_Init();
	ds1305_WriteBlock(0x0F,&tc,1);	// Enable timer and unwrite protect memory
	
}


#endif