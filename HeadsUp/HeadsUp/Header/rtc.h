#ifndef _RTC_H_
#define _RTC_H_


#define SS_PIN			PB4		/* Slave Select */
#define DD_SS_PIN		DDB4
#define MOSI_PIN		PB5		/* Data Out */
#define DD_MOSI_PIN		DDB5
#define MISO_PIN		PB6		/* Data In */
#define DD_MISO_PIN		DDB6
#define SCK_PIN			PB7		/* Clock */
#define DD_SCK_PIN		DDB7


typedef ds1305_Time ds1305_BCDTime;


typedef struct _ds1305_Time{
	unsigned char ucSeconds;
	unsigned char ucMinutes;
	unsigned char ucHours;
	unsigned char ucDay;
	unsigned char ucDate;
	unsigned char ucMonth;
	unsigned char ucYear;
} ds1305_Time;

class rtc{

	public:
		rtc( ){ ds1305_Init( ); }
		ds1305_Time get( ){ return GetCurrentTime( ); }
		void set( ds1305_Time* dstSetTime ){ SetCurrentTime( dstSetTime ); }

	private:
		void ds1305_Init( );
		void ds1305_ReadBlock( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		void ds1305_WriteBlock( unsigned char iAddr, unsigned char* pBuffer, unsigned int iCount );
		ds1305_Time GetCurrentTime( );
		void SetCurrentTime( ds1305_Time* dstSetTime );
		unsigned char SPI_MasterTransmit( unsigned char cData );
		void ds1305_spi_deselect( );
		void ds1305_spi_select( );
};







#endif
