/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include "headsup.h"



int main(){
	
	rtc_alarm rtaAlarm0 { 0x00, 0x00, 0x80, 0x80 };
	rtc_alarm rtaAlarm1 { 0x80, 0x80, 0x80, 0x80 };

	
	HeadsUp_Init();
	
	rtcDS.alarm( RTC_ALARM_SET, RTC_ALARM_0, rtaAlarm0 );
	rtcDS.alarm( RTC_ALARM_SET, RTC_ALARM_1, rtaAlarm1 );
		
	Menu_Main();
	
	return 0;

}
