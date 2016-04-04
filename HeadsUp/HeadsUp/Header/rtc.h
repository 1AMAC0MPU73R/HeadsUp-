#ifndef _RTC_H_
#define _RTC_H_

#include <string.h>


#define SS_PIN			PB4		/* Slave Select */
#define DD_SS_PIN		DDB4
#define MOSI_PIN		PB5		/* Data Out */
#define DD_MOSI_PIN		DDB5
#define MISO_PIN		PB6		/* Data In */
#define DD_MISO_PIN		DDB6
#define SCK_PIN			PB7		/* Clock */
#define DD_SCK_PIN		DDB7


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
		void init( );
		void read_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		void write_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		rtc_time get_time( );
		void set_time( rtc_time* rtmSetTime );
		unsigned char spi_master_transmit( unsigned char cData );
		void spi_deselect( );
		void spi_select( );
};







#endif
