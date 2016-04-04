/*
 * sdcard.h
 *
 * Created: 4/4/2016 7:08:26 PM
 *  Author: Jack
 */ 

#ifndef SDCARD_H_
#define SDCARD_H_

#include <avr/io.h>
#include <util/delay.h>

#include "fat16.h"

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define MOSI (1<<PB5)
#define MISO (1<<PB6)
#define SCK (1<<PB7)

#define CS_DDR DDRB
#define CS (1<<PB4)
#define CS_ENABLE() (PORTB &= ~CS)
#define CS_DISABLE() (PORTB |= CS)

void SPI_init();
unsigned char SPI_write(unsigned char ch);

unsigned char SD_command(unsigned char cmd, unsigned long arg, unsigned char crc, unsigned char read);
char SD_init();
void SD_read(unsigned long sector, unsigned short offset, unsigned char * buffer, unsigned short len);

void fat16_seek(unsigned long offset);
char fat16_read(unsigned char bytes);

int sd_display_file();

#endif /* SDCARD_H_ */