#include "rtc.h"


rtc::rtc(){
	
	uint8_t valCmd_Enable_W { 0x00 };
	uint8_t valCmd_Init_Control { 0x04 };
	uint8_t valCmd_Zero { 0x00 };
	rtc_time rtmInital { 0x00, 0x00, 0x52, 0x01, 0x01, 0x01, 0x00 };


	i2c_init();    // Establish I2C connection
	
	write_block( RTC_ADDR_CON, &valCmd_Enable_W, 1 );	// Enable timer and disable 'write protect'
	
	for( unsigned int valN = RTC_ADDR_START; valN <= RTC_ADDR_END; valN++ ){
		write_block( valN, &valCmd_Zero, 1 );	// Initialize timer to 0
	}
	
	write_block( RTC_ADDR_CON, &valCmd_Init_Control, 1 );	// Set exclusive interrupt behavior
	
	set_time(&rtmInital);    	// Initialize the time
	
}


void rtc::spi_select( ) {

	SPCR |= ( 1 << CPHA );
	PORTB |= ( 1 << SS_PIN );

}


/*
*	This routine de-selects the device by lowering the CE
*	line. Must be done after the master transmit is complete
*/
void rtc::spi_deselect( ){

	PORTB &= ~( 1 << SS_PIN );

}


void rtc::i2c_init( ){

	char IOReg;

	/* Outputs: MOSI and SCK out, all others inputs */
	DDRB = ( 1 << DD_MOSI_PIN ) | ( 1 << DD_SCK_PIN ) | ( 1 << DD_SS_PIN );
	/* Enable SPI, Master, CPOL=0, CPHA=1, set clock rate fck/16 */
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) | ( 1 << SPR0 );	// POSSIBLE ERROR: PULLING DATA HIGH
	
	/* Clear the SPIF bit in SPSR */
	IOReg = SPSR;
	IOReg = SPDR;

	spi_deselect( );
}


// This routine exchanges 1 byte with the SPI port. 
unsigned char rtc::spi_master_transmit( unsigned char cData )
{

	SPDR  = cData;	// Set Pointer to beginning of String
	/* wait until Char is sent */
	while ( ! (SPSR & ( 1 << SPIF ))){
		;	
	}

	return SPDR;
}


/*
*	ReadBlock reads iCount bytes starting at cAddr into the
*	buffer pBuffer
*/
void rtc::read_block( unsigned char cAddr, unsigned char *pBuffer, unsigned int iCount ){
	
	spi_select( );

	spi_master_transmit( cAddr );
	while( iCount-- ){
		*pBuffer++ = spi_master_transmit( 0 );
	}

	spi_deselect();

}


/*
*  WriteBlock sends iCount bytes from the buffer pBuffer
*  to the device at iAddr + 0x80. The device uses a split
*  memory map where all write addresses are at address +0x80
*/
void rtc::write_block(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount){
	
	unsigned char uchTempControlA { 0x00 };
	unsigned char uchTempControlB { 0x00 };
	
	
	read_block( RTC_ADDR_CON, &uchTempControlA, 1 );
	if( ! (( uchTempControlA & 0x03 ) == 0x00 )){
		uchTempControlB = uchTempControlA & 0xFC;
		spi_select( );

		spi_master_transmit( RTC_ADDR_CON + RTC_ADDR_W_OFF );
		spi_master_transmit( uchTempControlB );

		spi_deselect( );
	}
	
	spi_select( );

	spi_master_transmit( iAddr + RTC_ADDR_W_OFF );
	while( iCount-- ){
		spi_master_transmit( *pBuffer++ );
	}

	spi_deselect( );
	
	if( ! ( iAddr == RTC_ADDR_CON )){
		spi_select( );

		spi_master_transmit( RTC_ADDR_CON + RTC_ADDR_W_OFF );
		spi_master_transmit( uchTempControlA );

		spi_deselect( );
	}
}


rtc_time rtc::get_time( ){

	rtc_time rtmToConvert;


	read_block( RTC_ADDR_START, ( unsigned char * )&rtmToConvert, sizeof( rtc_time ));

	return rtmToConvert;
}


void rtc::set_time( rtc_time* rtmSetTime ){

	write_block( RTC_ADDR_START, ( unsigned char * )rtmSetTime, sizeof(rtc_time));
}


void rtc::rtm_to_char( rtc_time rtmToConvert, unsigned char* uchToReturn, unsigned int valCharLen ){
	
	unsigned char uchTimeOut[ valCharLen ];
	uint8_t valTimeLength = RTC_DISP_LEN;
	unsigned int valTextOffset = ( valCharLen - valTimeLength ) / 2;
	minute valMins;
	hour valHours;
	period valAMPM;
	uint16_t valDays;
	day valDay;
	uint32_t valMonths;
	month valMonth;
	date valDates;

	
	for(unsigned int valN = 0; valN < valCharLen; valN++ ){
		uchTimeOut[valN] = ' ';
	}

	valMins &= 0x0000;
	valMins |= (( rtmToConvert.ucMinutes & 0xF0 ) + 0x0300 ) << 4;
	valMins |= ( rtmToConvert.ucMinutes & 0x0F) + 0x0030;
	valHours &= 0x0000;
	valHours |= (( rtmToConvert.ucHours & 0x10 ) + 0x0300 ) << 4;
	valHours |= ( rtmToConvert.ucHours & 0x0F) + 0x0030;
	if(( rtmToConvert.ucHours & RTC_12PM ) == RTC_12AM ){
		valAMPM = 'A';
	}else{
		valAMPM = 'P';
	}
	valDay &= 0x00;
	valDays &= 0x00000000;
	valDay = ((( rtmToConvert.ucDay >> 4 ) & 0x0F ) * 10 ) + (( rtmToConvert.ucDay ) & 0x0F );
	valDays = chrDays[(( valDay == 0 ) ? 0 : valDay - 1 ) * 2];
	valDays |= chrDays[((( valDay == 0 ) ? 0 : valDay - 1 ) * 2 ) + 1] << 8;
	valDays |= ( static_cast<uint16_t>( chrDays[((( valDay == 0 ) ? 0 : valDay - 1 ) * 2 ) + 2])) << 16;
	valMonth &= 0x00;
	valMonths &= 0x00000000;
	valMonth = ((( rtmToConvert.ucMonth >> 4 ) & 0x0F ) * 10 ) + (( rtmToConvert.ucMonth ) & 0x0F );
	valMonths = chrMonths[(( valMonth == 0 ) ? 0 : valMonth - 1 ) * 3];
	valMonths |= chrMonths[((( valMonth == 0 ) ? 0 : valMonth - 1 ) * 3 ) + 1] << 8;
	valMonths |= ( static_cast<uint32_t>( chrMonths[((( valMonth == 0 ) ? 0 : valMonth - 1 ) * 3 ) + 2])) << 16;
	valDates &= 0x0000;
	valDates |= (( rtmToConvert.ucDate & 0xF0 ) + 0x0300 ) << 4;
	valDates |= ( rtmToConvert.ucDate & 0x0F) + 0x0030;
	
	uchTimeOut[valTextOffset] = valHours >> 8;
	uchTimeOut[valTextOffset + 1] = valHours & 0x00FF;
	uchTimeOut[valTextOffset + 2] = ':';
	uchTimeOut[valTextOffset + 3] = valMins >> 8;
	uchTimeOut[valTextOffset + 4] = valMins & 0x00FF;
	uchTimeOut[valTextOffset + 5] = valAMPM;
	uchTimeOut[valTextOffset + 6] = 'M';
	uchTimeOut[valTextOffset + 7] = ' ';
	uchTimeOut[valTextOffset + 8] = valDays & 0xFF;
	uchTimeOut[valTextOffset + 9] = ( valDays >> 8 ) & 0xFF;
	uchTimeOut[valTextOffset + 10] = ',';
	uchTimeOut[valTextOffset + 11] = valMonths & 0xFF;
	uchTimeOut[valTextOffset + 12] = ( valMonths >> 8 ) & 0xFF;
	uchTimeOut[valTextOffset + 13] = ( valMonths >> 16 ) & 0xFF;
	uchTimeOut[valTextOffset + 14] = valDates & 0x00FF;
	uchTimeOut[valTextOffset + 15] = valDates >> 8;

	memcpy( uchToReturn, uchTimeOut, valCharLen );

}

void rtc::alarm_on( uint8_t valAlarm01 ){
	
	unsigned char uchTempControl { 0x00 };


	if( valAlarm01 == RTC_ALARM_BOTH ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl |= 0x03;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}else if( valAlarm01 == RTC_ALARM_0 ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl |= 0x01;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}else if( valAlarm01 == RTC_ALARM_1 ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl |= 0x02;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}
	
}


void rtc::alarm_off( uint8_t valAlarm01 ){
	
	unsigned char uchTempControl { 0x00 };


	if( valAlarm01 == RTC_ALARM_BOTH ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl &= 0xFC;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}else if( valAlarm01 == RTC_ALARM_0 ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl &= 0xFE;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}else if( valAlarm01 == RTC_ALARM_1 ){
		read_block( RTC_ADDR_CON, &uchTempControl, 1 );
		uchTempControl &= 0xFD;
		write_block( RTC_ADDR_CON, &uchTempControl, 1);
	}

}


void rtc::clear_interupt( uint8_t valAlarm01 ){
	
	unsigned char chrNull;
	
	
	if( valAlarm01 == RTC_ALARM_0 ){
		read_block( RTC_ADDR_A0S, &chrNull, 1 );
	}else if( valAlarm01 == RTC_ALARM_1 ){
		read_block( RTC_ADDR_A1S, &chrNull, 1 );
	}else if( valAlarm01 == RTC_ALARM_BOTH ){
		read_block( RTC_ADDR_A0S, &chrNull, 1 );
		read_block( RTC_ADDR_A1S, &chrNull, 1 );
	}
	
}


void rtc::alarm( uint8_t valGetSet, uint8_t valAlarm01, rtc_alarm rtaToSetOrGet ){
	
	unsigned char valAlarmAddress { 0x00 };
	unsigned char uchTempControl { 0x00 };


	if( !valAlarm01 ){
			valAlarmAddress = RTC_ADDR_A0S;
	}else if( valAlarm01 ){
		valAlarmAddress = RTC_ADDR_A1S;
	}
	
	if ( !valGetSet ){
		read_block( valAlarmAddress, ( unsigned char* )&rtaToSetOrGet, sizeof( rtc_alarm ));
	}else if( valGetSet ){
		alarm_off( valAlarm01 );
		write_block( valAlarmAddress, ( unsigned char* )&rtaToSetOrGet, sizeof( rtc_alarm ));
		alarm_on( valAlarm01 );
	}
}


