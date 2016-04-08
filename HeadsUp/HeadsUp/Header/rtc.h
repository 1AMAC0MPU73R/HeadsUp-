#ifndef _RTC_H_
#define _RTC_H_

#include <string.h>

struct rtc_time{
	unsigned char ucSeconds;
	unsigned char ucMinutes;
	unsigned char ucHours;
	unsigned char ucDay;
	unsigned char ucDate;
	unsigned char ucMonth;
	unsigned char ucYear;
};

class rtc{

	public:
		rtc( );
		rtc_time get( ){ return get_time( ); }
		void set( rtc_time* rtmSetTime ){ set_time( rtmSetTime ); }
		void rtm_to_char( rtc_time rtmToConvert, unsigned char* uchToReturn, unsigned int valCharLen );

	private:
		void read_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		void write_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		rtc_time get_time( );
		void set_time( rtc_time* rtmSetTime );
};


#endif
