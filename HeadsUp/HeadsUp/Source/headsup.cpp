/*--------------- H E A D S U P . c p p ---------------
 
		   		   ANDREW GAUVIN

---------------------------------------------------*/

 
#include <avr/io.h>
#include <util/delay.h>
#include "headsup.h"


using namespace std;


int main(){
	
	Test_On_PortA0();
	
	test_LCD_Screen();
	
	Start_RTC();
	
	ds1305_Time dstTime;
	
	GetCurrentTime(&dstTime);

	return 0;	
}