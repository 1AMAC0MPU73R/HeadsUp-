/* ---------- H E A D S U P . H --------- */


#ifndef _HEADSUP_H_
#define _HEADSUP_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "rtc.h"
#include "lcd.h"
#include "bat.h"
#include "led.h"
#include "but.h"


#define F_CPU 8000000L		// 8 megaHz

#define CURSOR_OFF 0x00
#define CURSOR_ON 0xFF
#define CURSOR_DIV 0x11
#define MENU_LEN_MAX 16
#define MENU_WID_MAX 16
#define MENU_PAD_ON 0xFF
#define MENU_PAD_OFF 0x00
#define MAIN_STATE_ALARMS 0x01
#define MAIN_STATE_SETTINGS 0x02
#define MAIN_LENGTH 3
#define MAIN_MAX 7
#define ALARM_STATE_SECONDS 0x01
#define ALARM_STATE_MINUTES 0x02
#define ALARM_STATE_PERIOD 0x03
#define ALARM_LENGTH 3
#define ALARM_MAX 3
#define STARTUP_TEXT_TOP " -- HeadsUp! -- "
#define STARTUP_TEXT_BOT "Sleep Enviroment"


void HeadsUp_Init();
void Test_On_PortA0();
void Enable_PCINT( uint8_t valPC );
void Menu_Main();
void Menu_Alarm();
void Menu_Time();
void build_menu_main( const char vchStates[ MENU_LEN_MAX ][ MENU_WID_MAX ], unsigned char* chrOut, uint8_t valCursPos, uint8_t valPad, uint8_t valLen, uint8_t valWid );
void build_menu_alarm( const char vchStates[ MENU_LEN_MAX ][ MENU_WID_MAX ], unsigned char* chrOut, uint8_t valCursPos, uint8_t valPad, uint8_t valLen, uint8_t valWid );
void build_menu_time( const char vchStates[ MENU_LEN_MAX ][ MENU_WID_MAX ], unsigned char* chrOut, uint8_t valCursPos, uint8_t valPad, uint8_t valLen, uint8_t valWid );


#endif
