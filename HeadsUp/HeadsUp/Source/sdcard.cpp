#include <avr/io.h>

#include <util/delay.h>

#include "fat16.h"
#include "sdcard.h"

void SPI_init() {
	CS_DDR |= CS; // SD card circuit select as output
	SPI_DDR |= MOSI + SCK; // MOSI and SCK as outputs
	SPI_PORT |= MISO; // pullup in MISO, might not be needed
	
	// Enable SPI, master, set clock rate fck/128
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);
}

unsigned char SPI_write(unsigned char ch) {
	SPDR = ch;
	while(!(SPSR & (1<<SPIF))) {}
	return SPDR;
}

unsigned char SD_command(unsigned char cmd, unsigned long arg, unsigned char crc, unsigned char read) {
	unsigned char i, buffer[32], ret = 0xFF;
	
	//uwrite_str("CMD ");
	//uwrite_hex(cmd);
	
	CS_ENABLE();
	SPI_write(cmd);
	SPI_write(arg>>24);
	SPI_write(arg>>16);
	SPI_write(arg>>8);
	SPI_write(arg);
	SPI_write(crc);
	
	for(i=0; i<read; i++)
	buffer[i] = SPI_write(0xFF);
	
	CS_DISABLE();
	
	for(i=0; i<read; i++) {
		//USARTWriteChar(' ');
		//uwrite_hex(buffer[i]);
		if(buffer[i] != 0xFF)
		ret = buffer[i];
	}
	
	//uwrite_str("\r\n");
	
	return ret;
}

unsigned long sd_sector;
unsigned short sd_pos;


char SD_init() {
	char i;
	
	// ]r:10
	CS_DISABLE();
	for(i=0; i<10; i++) // idle for 1 bytes / 80 clocks
	SPI_write(0xFF);
	
	// [0x40 0x00 0x00 0x00 0x00 0x95 r:8] until we get "1"
	for(i=0; i<10 && SD_command(0x40, 0x00000000, 0x95, 8) != 1; i++)
	_delay_ms(100);
	
	if(i == 10) // card did not respond to initialization
	return -1;
	
	// CMD1 until card comes out of idle, but maximum of 10 times
	for(i=0; i<10 && SD_command(0x41, 0x00000000, 0xFF, 8) != 0; i++)
	_delay_ms(100);

	if(i == 10) // card did not come out of idle
	return -2;
	
	// SET_BLOCKLEN to 512
	SD_command(0x50, 0x00000200, 0xFF, 8);
	
	sd_sector = sd_pos = 0;
	
	return 0;
}

// TODO: This function will not exit gracefully if SD card does not do what it should
void SD_read(unsigned long sector, unsigned short offset, unsigned char * buffer, unsigned short len) {
	unsigned short i, pos = 0;
	
	CS_ENABLE();
	SPI_write(0x51);
	SPI_write(sector>>15); // sector*512 >> 24
	SPI_write(sector>>7);  // sector*512 >> 16
	SPI_write(sector<<1);  // sector*512 >> 8
	SPI_write(0);          // sector*512
	SPI_write(0xFF);
	
	for(i=0; i<100 && SPI_write(0xFF) != 0x00; i++) {} // wait for 0
	
	for(i=0; i<100 && SPI_write(0xFF) != 0xFE; i++) {} // wait for data start
	
	for(i=0; i<offset; i++) // "skip" bytes
	SPI_write(0xFF);
	
	for(i=0; i<len; i++) // read len bytes
	buffer[i] = SPI_write(0xFF);
	
	for(i+=offset; i<512; i++) // "skip" again
	SPI_write(0xFF);
	
	// skip checksum
	SPI_write(0xFF);
	SPI_write(0xFF);

	CS_DISABLE();
}

void fat16_seek(unsigned long offset) {
	sd_sector = offset >> 9;
	sd_pos = offset & 0x1FF;
}

char fat16_read(unsigned char bytes) {
	SD_read(sd_sector, sd_pos, fat16_buffer, bytes);
	sd_pos+=(unsigned short)bytes;
	
	if(sd_pos == 512) {
		sd_pos = 0;
		sd_sector++;
	}
	
	return bytes;
}

int sd_display_file() {
	char i;
	char ret;
	short offset = 0x1B0;

	SPI_init();
	
	//uwrite_str("Start\r\n");
	
	if(ret = SD_init()) {
		//uwrite_str("SD err: ");
		//uwrite_hex(ret);
		return -1;
	}
	
	if(ret = fat16_init()) {
		//uwrite_str("FAT err: ");
		//uwrite_hex(ret);
		return -1;
	}
	
	if(ret = fat16_open_file("README  ", "TXT")) {
		//uwrite_str("Open: ");
		//uwrite_hex(ret);
		return -1;
	}

	while(fat16_state.file_left) {
		ret = fat16_read_file(FAT16_BUFFER_SIZE);
		for(i=0; i<ret; i++) {
			fat16_buffer[i];
		}
	}

	return 0;
}