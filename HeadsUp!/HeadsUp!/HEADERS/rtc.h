#ifndef _RTC_H_
#define _RTC_H_

#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define SS_PIN			PB4		/* Slave Select */
#define DD_SS_PIN		DDB4
#define MOSI_PIN		PB5		/* Data Out */
#define DD_MOSI_PIN		DDB5
#define MISO_PIN		PB6		/* Data In */
#define DD_MISO_PIN		DDB6
#define SCK_PIN			PB7		/* Clock */
#define DD_SCK_PIN		DDB7

#define RTC_ADDR_START 0x00
#define RTC_ADDR_END 0x10
#define RTC_ADDR_W_OFF 0x80
#define RTC_ADDR_SEC 0x00
#define RTC_ADDR_MIN 0x01
#define RTC_ADDR_HOU 0x02
#define RTC_ADDR_DAY 0x03
#define RTC_ADDR_DAT 0x04
#define RTC_ADDR_MON 0x05
#define RTC_ADDR_YEA 0x06
#define RTC_ADDR_A0S 0x07
#define RTC_ADDR_A0M 0x08
#define RTC_ADDR_A0H 0x09
#define RTC_ADDR_A0D 0x0A
#define RTC_ADDR_A1S 0x0B
#define RTC_ADDR_A1M 0x0C
#define RTC_ADDR_A1H 0x0D
#define RTC_ADDR_A1D 0x0E
#define RTC_ADDR_CON 0x0F
#define RTC_ADDR_STA 0x10

#define RTC_ALARM_GET 0
#define RTC_ALARM_SET 1
#define RTC_ALARM_0 0
#define RTC_ALARM_1 1
#define RTC_ALARM_BOTH 2

#define RTC_DISP_LEN 16    // LEN( '12:45AM MO,APR11' ) = 16

#define RTC_12AM 0x40			/* TIME DEFINITIONS */
#define RTC_12PM 0x60
#define RTC_SUN 0x01
#define RTC_MON 0x02
#define RTC_TUE 0x03
#define RTC_WED 0x04
#define RTC_THU 0x05
#define RTC_FRI 0x06
#define RTC_SAT 0x07
#define RTC_JAN 0x01
#define RTC_FEB 0x02
#define RTC_MAR 0x03
#define RTC_APR 0x04
#define RTC_MAY 0x05
#define RTC_JUN 0x06
#define RTC_JUL 0x07
#define RTC_AUG 0x08
#define RTC_SEP 0x09
#define RTC_OCT 0x10
#define RTC_NOV 0x11
#define RTC_DEC 0x12


typedef uint16_t year;		/* TIME TYPES */
typedef uint16_t month;
typedef uint16_t date;
typedef uint8_t day;
typedef uint16_t hour;
typedef uint16_t minute;
typedef uint16_t second;
typedef uint8_t period;


const unsigned char chrMonths[36] { 'J', 'A', 'N' ,'F', 'E', 'B' ,'M', 'A', 'R' ,'A', 'P', 'R' ,'M', 'A', 'Y' ,'J', 'U', 'N' ,'J', 'U', 'L' ,'A', 'U', 'G' ,'S', 'E', 'P' ,'O', 'C', 'T' ,'N', 'O', 'V' ,'D', 'E', 'C' };
const unsigned char chrDays[14] { 'S', 'U', 'M' ,'O', 'T', 'U' ,'W', 'E', 'T' ,'H', 'F', 'R' ,'S', 'A' };


struct rtc_time{
	unsigned char ucSeconds;
	unsigned char ucMinutes;
	unsigned char ucHours;
	unsigned char ucDay;
	unsigned char ucDate;
	unsigned char ucMonth;
	unsigned char ucYear;
};


struct rtc_alarm{
	unsigned char ucMinutes;
	unsigned char ucHours;
};


class rtc{

	public:
		rtc();
		rtc_time get(){ return get_time(); }
		void set( rtc_time* rtmSetTime ){ set_time( rtmSetTime ); }
		void rtm_to_char( rtc_time rtmToConvert, unsigned char* chrToReturn, unsigned int valCharLen );
		void char_to_alarm( unsigned char* chrToConvert, rtc_alarm* almToReturn );
		void char_to_rtm( unsigned char* chrToConvert, rtc_time* tmeToReturn);
		void alarm( uint8_t valGetSet, uint8_t valAlarm01, rtc_alarm rtaToSetOrGet );
		void alarm_on( uint8_t valAlarm01 );
		void alarm_off( uint8_t valAlarm01 );
		void clear_interupt( uint8_t valAlarm01 );

	private:
		void i2c_init();
		void read_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		void write_block( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		rtc_time get_time();
		void set_time( rtc_time* rtmSetTime );
		unsigned char spi_master_transmit( unsigned char cData );
		void spi_deselect();
		void spi_select();
};







#endif
