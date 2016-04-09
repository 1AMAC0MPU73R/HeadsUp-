/*
 * spi.h
 *
 * Created: 4/8/2016 12:20:35 PM
 *  Author: Jack
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define SS_PIN			PB4		/* Slave Select */
#define DD_SS_PIN		DDB4
#define MOSI_PIN		PB5		/* Data Out */
#define DD_MOSI_PIN		DDB5
#define MISO_PIN		PB6		/* Data In */
#define DD_MISO_PIN		DDB6
#define SCK_PIN			PB7		/* Clock */
#define DD_SCK_PIN		DDB7

/* Function Prototypes */


void spi_deselect( );
void spi_select( );
void spi_init( );
unsigned char spi_master_transmit( unsigned char cData );
unsigned char spi_master_read(void);


#endif /* SPI_H_ */