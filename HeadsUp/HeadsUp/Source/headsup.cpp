/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include <avr/io.h>
#include <util/delay.h>
#include "headsup.h"


using namespace std;


int main(){
	
	ds1305_Time dstTime;
	lcd lcdNHD;
	

	Test_On_PortA0();
	lcdNHD.test_LCD_Screen();
	
	Start_RTC();
	
	unsigned char seco[16];
	
	for(;;){
		GetCurrentTime(&dstTime);
		seco[0] = ((( dstTime.ucSeconds & 0xF0 ) >> 4 ) + '0');
		seco[1] = (( dstTime.ucSeconds & 0xF ) + '0' );
		lcdNHD.print(seco, LINE_TOP);
		_delay_ms(1000);
	}
	
	return 0;	
}