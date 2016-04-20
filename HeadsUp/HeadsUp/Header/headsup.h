/* ---------- H E A D S U P . H --------- */


#ifndef _HEADSUP_H_
#define _HEADSUP_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "rtc.h"
#include "lcd.h"
#include "bat.h"
#include "led.h"

#define F_CPU 8000000L		// 8 megaHz

#define CURSOR_OFF 0x00
#define CURSOR_ON 0xFF
#define CURSOR_DIV 0x0A
#define MAIN_STATE_ALARMS 0
#define MAIN_STATE_SETTINGS 1



void HeadsUp_Init();
void Test_On_PortA0();
void Enable_PCINT2();
void Enable_PCINT3();
void Menu_Main();

#endif
