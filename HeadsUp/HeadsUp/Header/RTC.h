/*
 *  RTC.h
 *
 * Created: 3/4/2016 1:28:16 PM
 * Author: Jack
 */ 


#ifndef _RTC_H_
#define _RTC_H_


//
// ds1305.h is the shared header file for the file ds1305.c
//
// See the complete file and article that goes with it at
//
// www.seattlerobotics.org/encoder/200005/ds1305.html
//

typedef struct _ds1305_Time
{
	unsigned char ucSeconds;
	unsigned char ucMinutes;
	unsigned char ucHours;
	unsigned char ucDay;
	unsigned char ucDate;
	unsigned char ucMonth;
	unsigned char ucYear;
} ds1305_Time;

typedef ds1305_Time ds1305_BCDTime;

void ds1305_Init();

void ds1305_ReadBlock(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount);
void ds1305_WriteBlock(unsigned char iAddr, unsigned char *pBuffer, unsigned int iCount);


#define GetCurrentTime(pds1305_Time) ds1305_ReadBlock( 0, (unsigned char *)pds1305_Time,sizeof(ds1305_Time))
#define SetCurrentTime(pds1305_Time) ds1305_WriteBlock( 0, (unsigned char *)pds1305_Time,sizeof(ds1305_Time))


#endif