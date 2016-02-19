/*********** AVR155 : Accessing I2C LCD display using the AVR TWI ***********

Device      : ATmega163

File name   : LCD_control.h

Ver nr.     : 1.0

Description : Header file for LCD_control.c

Author      : Asmund Saetre

Change log  : Created 02.05.2000  AS
 
****************************************************************************/

/****************************************************************************
	Include files
****************************************************************************/
#include <iom163.h>							//Change according to device
#include "ina90.h"
#include "pgmspace.h"
#include "TWI_driver.h"

/****************************************************************************
	Function definitions
****************************************************************************/
unsigned char Send_to_TWI(tx_type*);		//TWI master driver, controlling
											// TX/RX on the bus
unsigned char Setup(void);					//General setup for the AVR
unsigned char Shift_left(void);				//Shift LCD text left	
unsigned char Init_LCD(void);				//Initilaize LCD
unsigned char Clear_LCD(void);				//Write blanks to all LCD pos.
unsigned char Read_read_controlbyte(void);	//Read the LCDs Read Control
											//Registry
void Delay1mS(int number_of_mS);			//1 mS delay loop	



/****************************************************************************
	Bit and byte definitions
****************************************************************************/
#define	BUSY_FLAG			0x80			//Bit pos for display Busyflag
#define	DDRAM_ADR			0x80			//Write to DDRAM AC
#define DDRAM_WR			0x40			//Write to DDRAM
#define	FUNC_SET			0x00			//Enter LCD Function settings
#define NR_CHAR				80             	//How may letter memory locations
#define NR_ROW  	   		2				//Number of display rows
#define CHAR_PER_ROW  (NR_CHAR/NR_ROW)      //Number of characters on a row
#define ASCII_OFFSET		0x80			//The driver(Philips 2119) offset
											//according to the ASCII table
#define LCD_ADR 			0x76  			//LCD display adr, SA0 = GND
#define RS_DATA 			0xC0  			//Register Selcet data. Write
											//or read LCD DDRAMs data
#define RS_CONT 			0x80  			//Register Select Control Write 
											//or read LCD Control reg
#define CLR_ADR 			0x02  			//Set DDRAM AC to 0 and return 
											//shifted display to it's 
											//original Position
#define CLR_LCD 			0x01  			//Clear DDRAM and set DDRAM
											//AC to 0
#define SHIFT_LEFT			0x18			//Shift the displayed text once
											//to the left
#define READ_CONT_REG		0x00			//Setup for reading Busy Flag and
											//DDRAM address counter	 			