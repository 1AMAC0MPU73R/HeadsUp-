/*
*	Sample program Fez stole from the Internet found at http://www.seattlerobotics.org/encoder/200005/ds1305t.c
*
*	This simple programs job is to initialize the ds1305, then sit
*	in a loop and print out the Time/Date every second or so. The seconds
*	timer uses the output from the time keeping chip.
*/
//
// ds1305t.c is a simple test program that uses the ds1305 as a real
// time clock. This program is intended to show how to use the
// real time clock functions found in ds1305.c and ds1305.h files.
//
// You will notice that I didn't use the printf function in this file.
// The Imagecraft printf function sucks in a fairly large library that
// exceeds the memory limits of the 68HC811E2. Therefore, I left it out
// in favor of the puts and BCDOutput functions. If you are using a
// larger memory chip, like the HC12, or an HC11 with expanded memory,
// you can use printf if you would like.
//
// This program was written to go with an article about the ds1305 in
// the May, 2000 issue of the Encoder. www.seattlerobotics.org/encoder/200005
//
// 04/30/00	kevinro@nwlink.com
//

/* CPU Clock Freq */
#define F_CPU	1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ds1305.h"

//
// BCDOutput takes a BCD encoded byte (c) and prints the value out. The
// output is in decimal, and will always consist of two characters.
//
void BCDOutput(unsigned char c)
{
	putchar( ((c & 0xF0) >> 4) + '0');
	putchar( (c & 0xF) + '0');
}

//
// The pragma here places the data segment inside the text segment. If
// you have constant strings, this is a good thing to do so they are
// not placed in RAM.
//
#pragma data:text

char *pDates[] = { "Inv","Sun","Mon","Tue","Wed","Thu","Fri","Sat" };

#pragma data:data


//
// This function outputs a ds1305_Time structure to the console.
//
//
void OutputTimeDate(ds1305_Time *ptd)
{
	// Start by outputting date fields
	
	puts(pDates[(ptd->ucDay) & 0x07 ]);
	putchar(' ');
	BCDOutput(ptd->ucMonth);
	putchar('/');
	BCDOutput(ptd->ucDate);
	putchar('/');
	BCDOutput(ptd->ucYear);
	
	putchar(' ');
	BCDOutput(ptd->ucHours & 0x3F);
	putchar(':');
	BCDOutput(ptd->ucMinutes);
	putchar(':');
	BCDOutput(ptd->ucSeconds);
	putchar('\n');
}

//
// This simple programs job is to initialize the ds1305, then sit
// in a loop and print out the Time/Date every second or so. The seconds
// timer uses the output from the time keeping chip.
//
int main()
{
	ds1305_Time dsTime;
	unsigned char LastSeconds;
	unsigned char tc;
	
	//puts("ds1305t\n");
	ds1305_Init();
	//
	// Enable timer and unwrite protect memory
	//
	tc = 0;
	ds1305_WriteBlock(0x0F,&tc,1);
	
	
	do
	{
		GetCurrentTime(&dsTime);
		if(LastSeconds != dsTime.ucSeconds)
		{
			//OutputTimeDate(&dsTime);
			LastSeconds = dsTime.ucSeconds;
		}
		
	} while (1);
}

//
// A Reset vector for this program.
//
/*extern void _start(void);

#pragma abs_address:0xfffe
void (*interrupt_vectors[])(void) =
{
	_start
};

#pragma end_abs_address
*/