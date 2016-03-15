/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include <avr/io.h>
#include <util/delay.h>
#include "headsup.h"


int main(){
	
	ds1305_Time dstCurrentTime;
	rtc rtcDS;
	lcd lcdNHD;
	

	dstCurrentTime.ucSeconds = 0x00;
	dstCurrentTime.ucMinutes = 0x39;
	dstCurrentTime.ucHours = 0x10;
	
	Test_On_PortA0();
	lcdNHD.test_LCD_Screen();

	rtcDS.set(&dstCurrentTime);

	unsigned char timo[16] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
	uint16_t seco = 0x2020;
	uint16_t mino = 0x20;
	uint16_t houo = 0x20;
	
	for(;;){
		dstCurrentTime = rtcDS.get();
		seco &= 0x0000;
		seco |= (( dstCurrentTime.ucSeconds & 0xF0 ) + 0x0300 ) << 4;
		seco |= ( dstCurrentTime.ucSeconds & 0x0F) + 0x0030;
		mino &= 0x0000;
		mino |= (( dstCurrentTime.ucMinutes & 0xF0 ) + 0x0300 ) << 4;
		mino |= ( dstCurrentTime.ucMinutes & 0x0F) + 0x0030;
		houo &= 0x0000;
		houo |= (( dstCurrentTime.ucHours & 0xF0 ) + 0x0300 ) << 4;
		houo |= ( dstCurrentTime.ucHours & 0x0F) + 0x0030;
		timo[3] = houo >> 8;
		timo[4] = houo & 0x00FF;
		timo[5] = ':';
		timo[6] = mino >> 8;
		timo[7] = mino & 0x00FF;
		timo[8] = ':';
		timo[9] = seco >> 8;
		timo[10] = seco & 0x00FF;
		lcdNHD.print(timo, LINE_TOP);
		_delay_ms(1000);
	}
	
	return 0;

}
