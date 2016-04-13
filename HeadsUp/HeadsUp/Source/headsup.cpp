/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include "headsup.h"


rtc rtcDS;
lcd lcdNHD;
bat batLiPo;
rtc_time rtmCurrent{0x50, 0x59, 0x72, 0x02, 0x11, 0x04, 0x16};


int main(){
	
	rtc_alarm rtaAlarm0 { 0x00, 0x00, 0x80, 0x80 };
	rtc_alarm rtaAlarm1 { 0x80, 0x80, 0x80, 0x80 };

	
	HeadsUp_Init();
	
	rtcDS.alarm( RTC_ALARM_SET, RTC_ALARM_0, rtaAlarm0 );
	rtcDS.alarm( RTC_ALARM_SET, RTC_ALARM_1, rtaAlarm1 );
		
	Menu_Main();
	
	return 0;

}


void HeadsUp_Init(){
	
	unsigned char uchCurrentTime[ LCD_LINE_SIZE ] ;
	
	
	Test_On_PortA0();
	
	rtcDS.set(&rtmCurrent);

	Enable_PCINT2();
	Enable_PCINT3();
	sei();
	
	rtcDS.rtm_to_char( rtmCurrent, uchCurrentTime, LCD_LINE_SIZE );
	lcdNHD.print( uchCurrentTime , LCD_LINE_TOP );

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
	
	unsigned char chrMenuTop[LCD_LINE_SIZE];
	unsigned char chrMenuBot[LCD_LINE_SIZE]{ 'A', 'L', 'R', 'M', '|', 'B', 'A', 'T', '9', '3', '%', '|', 'O', 'P', 'T', 'S' };
	uint8_t valMenuState{ MAIN_STATE_ALARMS };
	uint8_t valCursorState{ CURSOR_OFF };
	uint8_t valCursorDivider{ 0 };
	
	lcdNHD.print( chrMenuBot , LCD_LINE_BOTTOM );
	
	for(;;){
		rtcDS.rtm_to_char( rtmCurrent, chrMenuTop, LCD_LINE_SIZE );
		lcdNHD.print( chrMenuTop , LCD_LINE_TOP );
		
		valCursorDivider = ( valCursorDivider + 1 ) % CURSOR_DIV;
		if( valCursorDivider == 0 ){
					valCursorState = ~valCursorState;
		}
		
		if( valCursorState == CURSOR_OFF ){
			chrMenuBot[0] = 'A';
			chrMenuBot[1] = 'L';
			chrMenuBot[2] = 'R';
			chrMenuBot[3] = 'M';
			chrMenuBot[12] = 'O';
			chrMenuBot[13] = 'P';
			chrMenuBot[14] = 'T';
			chrMenuBot[15] = 'S';
		}
		
		if( valMenuState == MAIN_STATE_ALARMS ){
			if( valCursorState == CURSOR_ON ){
				chrMenuBot[0] = '_';
				chrMenuBot[1] = '_';
				chrMenuBot[2] = '_';
				chrMenuBot[3] = '_';
				chrMenuBot[12] = 'O';
				chrMenuBot[13] = 'P';
				chrMenuBot[14] = 'T';
				chrMenuBot[15] = 'S';
			}
		}else if( valMenuState == MAIN_STATE_SETTINGS ){
			if( valCursorState == CURSOR_ON ){
				chrMenuBot[0] = '_';
				chrMenuBot[1] = '_';
				chrMenuBot[2] = '_';
				chrMenuBot[3] = '_';
				chrMenuBot[12] = '#';
				chrMenuBot[13] = '#';
				chrMenuBot[14] = '#';
				chrMenuBot[15] = '#';
			}
		}
		lcdNHD.print( chrMenuBot, LCD_LINE_BOTTOM );
		_delay_ms(50);
	}
	
}

