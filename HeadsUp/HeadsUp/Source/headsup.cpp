/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include <avr/io.h>
#include <util/delay.h>
#include "headsup.h"


int main(){
	
	rtc_time rtmCurrent;
	lcd lcdNHD;
	rtc rtcDS;
	unsigned char uchCurrenttime[ LINE_SIZE ] ;


	rtmCurrent.ucSeconds = 0x50;
	rtmCurrent.ucMinutes = 0x59;
	rtmCurrent.ucHours = 0x72;
	
	Test_On_PortA0();
	lcdNHD.test_LCD_Screen();

	rtcDS.set(&rtmCurrent);

	for(;;){
		rtmCurrent=rtcDS.get();
		rtcDS.rtm_to_char( rtmCurrent, uchCurrenttime, LINE_SIZE );
		lcdNHD.print( uchCurrenttime , LINE_TOP );
		_delay_ms( 1000 );
	}
	
	return 0;

}
