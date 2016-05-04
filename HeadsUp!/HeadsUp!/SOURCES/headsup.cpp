/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include "headsup.h"


rtc rtcDS;
lcd lcdNHD;
bat batLiPo;
led ledAlarm;
rtc_time rtmCurrent{0x50, 0x59, 0x72, 0x02, 0x11, 0x04, 0x16};
uint8_t valLastPress{ BUT_UP };


int main(){

	rtc_alarm rtaAlarm0 { 0x00, 0x00, 0x80, 0x80 };
	rtc_alarm rtaAlarm1 { 0x00, 0x80, 0x80, 0x80 };

	
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

	Enable_PCINT( 2 );
	Enable_PCINT( 3 );
	Enable_PCINT( 8 );
	Enable_PCINT( 9 );
	Enable_PCINT( 10 );
	sei();
	
	rtcDS.rtm_to_char( rtmCurrent, uchCurrentTime, LCD_LINE_SIZE );
	lcdNHD.print( uchCurrentTime , LCD_LINE_TOP );

}


void Enable_PCINT( uint8_t valPC ){
	
	if( valPC < 8 ){
		DDRA &= ~( 1 << ( valPC % 8 ));
		PORTA |= 1 << ( valPC % 8 );
		PCICR |= 1 << PCIE0;
		PCMSK0 |= 1 << ( valPC % 8 );
		PCIFR |= 1 << PCIF0;
	}else if( valPC < 16 ){
		DDRB &= ~( 1 << ( valPC % 8 ));
		PORTB |= 1 << ( valPC % 8 );
		PCICR |= 1 << PCIE1;
		PCMSK1 |= 1 << ( valPC % 8 );
		PCIFR |= 1 << PCIF1;
	}else if( valPC < 24 ){
		DDRC &= ~( 1 << ( valPC % 8 ));
		PORTC |= 1 << ( valPC % 8 );
		PCICR |= 1 << PCIE2;
		PCMSK2 |= 1 << ( valPC % 8 );
		PCIFR |= 1 << PCIF2;
	}else{
		DDRD &= ~( 1 << ( valPC % 8 ));
		PORTD |= 1 << ( valPC % 8 );
		PCICR |= 1 << PCIE3;
		PCMSK3 |= 1 << ( valPC % 8 );
		PCIFR |= 1 << PCIF3;
	}

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
	uint8_t valCounter{ 100 };
	uint8_t valIDelta{ valCounter / 100 };
	uint8_t valI{ 0 };

	
	memcpy(( void* )&chrPinA, ( void* )0x20, 1 );
	
	if((( chrPinA >> PORTA3 ) & 0x01 ) == 0x00 ){
		DDRA |= 1 << PORTA4;
		PORTA |= ( 1 << PINA4);
		_delay_ms(250);
		PORTA &= ~( 1 << PINA4);
		do{
			ledAlarm.set( valI );
			valI = valI + valIDelta;
			_delay_ms( 100 );
		}while( --valCounter );
		ledAlarm.set( 0 );
		rtcDS.clear_interupt( RTC_ALARM_0 );
	}else if((( chrPinA >> PORTA2 ) & 0x01 ) == 0x00 ){
		rtmCurrent = rtcDS.get();
		rtcDS.clear_interupt( RTC_ALARM_1 );
	}
	
}


void Menu_Main(){
	
	const char chrMenu[ MENU_LEN_MAX ][ MENU_WID_MAX ]{ "ALRM", "BAT  %", "OPTS" };
	unsigned char chrBat[2]{'0','0'};
	unsigned char chrMenuOut[ LCD_LINE_SIZE ];
	uint8_t valMenuState{ MAIN_STATE_ALARMS };
	uint8_t valCursorState{ CURSOR_OFF };
	uint8_t valCursorDivider{ 0 };
	uint16_t valBatUpdate{ 0 };

	extern uint8_t valLastPress;
	
	for(;;){	
		build_menu_main( chrMenu, chrMenuOut, valCursorState, MAIN_PAD_OFF, MAIN_LENGTH, MAIN_MAX );
		if( valBatUpdate == 0 ){
			chrBat[0] = ( batLiPo.get() >> 4 ) + 0x30;
			chrBat[1] = ( batLiPo.get() & 0x0F ) + 0x30;
		}
		memcpy( ( void* )( &chrMenuOut + 8 ), ( void* )&chrBat, sizeof( chrBat ));
		valBatUpdate = ( valBatUpdate + 1 ) % 100;
		lcdNHD.print( chrMenuOut, LCD_LINE_BOTTOM );
	
		if( valLastPress == 0x03 ){
			valMenuState = ( valMenuState + 0x03 ) % 0x02;
		}else if( valLastPress == 0x02 ){
			valMenuState = ( valMenuState + 0x01 ) % 0x02;
		}
		valLastPress = 0;
		
		valCursorDivider = ( valCursorDivider + 1 ) % CURSOR_DIV;
		if( valCursorDivider == 0 ){
					valCursorState = ~valCursorState;
		}
		_delay_ms(30);
	}
	
}


void build_menu_main( const char chpStates[MENU_LEN_MAX][MENU_WID_MAX], unsigned char* chrReturn, uint8_t valCursPos, uint8_t valPad, uint8_t valLen, uint8_t valWid ){
	
	unsigned char chrOutput[ LCD_LINE_SIZE ];
	uint8_t valDivPos{ 0 };
	uint8_t valCursStart{ 0 };
	uint8_t valCursEnd{ 0 };
	uint8_t valN{ 1 };
	
	
	valDivPos = (( strlen( chpStates[0] ) < 2 ) | ( sizeof( chrOutput ) <= strlen( chpStates[0] )) ? 0 : strlen( chpStates[0] ) + 1 );	
	for( uint8_t valR = 0; valR < valCursPos; valR++ ){
		if( valR == 0 ){
			valCursStart = 1;
		}
		valCursStart += strlen( chpStates[valR] );
		valCursEnd += strlen( chpStates[valR] );
		valCursStart++;
		valCursEnd++;
	}
	valCursEnd += strlen( chpStates[valCursPos] );
	valCursEnd++;
	valCursStart = ( valCursStart > sizeof( chrOutput ) ? 0 : valCursStart );
	valCursEnd = ( valCursStart == 0 ) ? 0 : ( valCursEnd >= sizeof( chrOutput )) ? sizeof( chrOutput ) : valCursEnd;
	
	valDivPos = ( valPad != MAIN_PAD_ON ) ? valDivPos : ( valDivPos >= ( sizeof( chrOutput ) - 1 )) ? 0 : ( valDivPos + 1 );
	valCursStart = ( valPad != MAIN_PAD_ON ) ? valCursStart : ( valCursStart + 1 );
	valCursEnd = ( valPad != MAIN_PAD_ON ) ? valCursEnd : ( valCursEnd - 1 );
	
	if( valPad == MAIN_PAD_ON ){
		chrOutput[ valN ] = ' ';
		valN++;
	}
	
	for( uint8_t valQ = 1; valQ <= valLen; valQ++ ){
		for( uint8_t valJ = 1; valJ <= valWid; valJ++ ){
			if( valN == valDivPos ){
				chrOutput[ valN - 1 ] = '|';
				valDivPos = (( sizeof( chrOutput ) - ( valDivPos + 1  )) < strlen( chpStates[valQ] )) ? 0 : ( valDivPos + strlen( chpStates[valQ] )) + 1;
				valDivPos = ( valPad != MAIN_PAD_ON ) ? valDivPos : ( valDivPos >= ( sizeof( chrOutput ) - 1 )) ? 0 : ( valDivPos + 1 );
			}else if(( valN >= valCursStart ) & ( valN <= valCursEnd )){
				chrOutput[ valN - 1 ] = '_';
			}else if(( valPad == MAIN_PAD_ON ) & ( valN == sizeof( chrOutput ))){
				chrOutput[ valN - 1 ] = ' ';
			}else{
				if( strlen( &chpStates[ valQ - 1 ][ valJ - 1 ] ) != 0 ){
					memcpy( ( void* )&chrOutput[ valN - 1 ], ( void* )&chpStates[ valQ - 1 ][ valJ - 1 ], 1);
				}else{
					valN--;
				}
			}
			if( valN < sizeof(chrOutput) ){
				valN++;
			}else{
				memcpy( chrReturn, chrOutput, sizeof( chrOutput ));
				return;
			}	
		}
	}
}