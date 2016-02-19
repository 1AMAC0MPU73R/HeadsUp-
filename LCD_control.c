/*********** AVR155 : Accessing I2C LCD display using the AVR TWI ***********

Device      : 	ATmega163

File name   : 	LCD_control.c

Ver nr.     : 	1.0

Description : 	This application note describes how to aproach the TWI bus
				system and to communicate with Philips LCD on a I2C bus. 
				It is divided into to parts and "LCD_control.c" setup and 
				control the LCD display. 
				
				It shows how to:
				-Set up the display
				-Write astring to the display
				-Shift the displayed text to the left to make a scrolling text.
				-Read stausinformation from the display

				For the communication against the I2C bus it uses the AVRs 
				hardware TWI module and access it through a driver. This driver
				is described in "TWI_driver.c"
				
				The application use a Batron BT21605V-SRE-I2C-COG module 
				with a onboard Philips PCF2119RU I2C "Chip on Glas" LCD driver.
                 
Author      : 	Asmund Saetre

Change log  : 	Created 12.05.2000  AS
 
****************************************************************************/
#include "LCD_control.h"

/****************************************************************************
	Text string definitions for demonstrations of display
****************************************************************************/
//flash char	String1[] ={"AVR mega163 TWI"};
flash char	String1[] ={"AVR ATmega163 with TWI interface to I2C"};

/****************************************************************************
	Function : char setup(void)									
	AVR setup routine
****************************************************************************/
char setup(void)
{	
	Init_TWI();								//Call the init TWI from 
											//TWI_driver.c
	return 1;									
}		 

/****************************************************************************
	Function : unsigned char Clear_LCD(void)						
	The Philips drivers internal ASCII table has a offset and the internal 
	clear display will not make the display look empty. To make the LCD look
	empty it requiers the DDRAM to contain "SPACE" + ASCII_OFFSET
****************************************************************************/
unsigned char Clear_LCD(void)
{	
	char temp0[2];
	char temp1[(CHAR_PER_ROW*NR_ROW)+1];
	char temp2[2];	
	char state,i;
	tx_type tx_frame[4];

	state = SUCCESS;
	
//Struct # 0 in the package	
	tx_frame[0].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[0].size = 2;					//Number of bytes to send
	tx_frame[0].data_ptr = temp0;			//Set the pointer to temp array	0
	tx_frame[0].data_ptr[0] = FUNC_SET;		//Enter function setting
	tx_frame[0].data_ptr[1] = CLR_LCD;		//Clear display and set DDRAM adr
											//counter to 0
//Struct # 1 in the package														
	tx_frame[1].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[1].size = ((CHAR_PER_ROW*NR_ROW)+1);//Number of bytes to send
	tx_frame[1].data_ptr = temp1;			//Set the pointer to temp array	1	
	tx_frame[1].data_ptr[0] = DDRAM_WR;		//Access DDRAM fro write or read

	//Fill DDRAM with SPACE + ASCII_OFFSET to make the display look empty		
	for(i=1;i<((CHAR_PER_ROW*NR_ROW)+1);i++)
		tx_frame[1].data_ptr[i] = (' '+ASCII_OFFSET);
							
//Struct # 2 in the package	
	tx_frame[2].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[2].size = 2;					//Number of bytes to send
	tx_frame[2].data_ptr = temp2;			//Set the pointer to temp array	2
	tx_frame[2].data_ptr[0] = RS_CONT;		//Access LCD Control Byte
	tx_frame[2].data_ptr[1] = CLR_ADR;		//Clear display and set DDRAM adr
											//counter to 0
//Struct # 3 in the package												
	tx_frame[3].slave_adr = OWN_ADR;		//Set to OWN_ADR to end TWI frame


	state =Send_to_TWI(tx_frame);			//Call the Master TWI driver with
											//a pointer to the first struct 
											//in the package
											
	return state;							//If error occured during the TWI
											//comunication, return TWSR
											//If  no error, return SUCCESS
}    

/****************************************************************************
	Function : unsigned char Init_LCD(void)					
	Setup the LCD to a 2 lines by 16 display with no cursor or blinking.
	Set the DDRAM adr counter to increment for each DDRAM write and do not 
	shift the display for each write. 
	Clear display and set DDRAM adr counter to 0.

****************************************************************************/
unsigned char Init_LCD(void)
{	
	char temp[5];
	char state;
	tx_type tx_frame[2];

	state = SUCCESS;
	
	tx_frame[0].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[0].size = 5;					//Number of bytes to send
	tx_frame[0].data_ptr = temp;			//Set the pointer a temp array	
	tx_frame[0].data_ptr[0] = FUNC_SET;		//Selcet function setting
	tx_frame[0].data_ptr[1] = 0x34;			//2 by 16 display
	tx_frame[0].data_ptr[2] = 0x0C;			//Turn display on with no cursor
	tx_frame[0].data_ptr[3] = 0x06;			//Inc cursor adr for each write
											//to DDRAM
	tx_frame[0].data_ptr[4] = CLR_LCD;		//Clear display and set DDRAM adr
											//counter to 0
													
	tx_frame[1].slave_adr = OWN_ADR;		//Set to OWN_ADR to end TWI frame


	state =Send_to_TWI(tx_frame);			//Call the Master TWI driver with
											//a pointer to the first struct 
											//in the package
											
	return state;							//If error occured during the TWI
											//comunication, return TWSR
											//If  no error, return SUCCESS

}    

/****************************************************************************
	Function : unsigned char Write_String1(void)
	Write a string to the LCD DDRAM and return the LCD text to it's start 
	position. i.e. DDRAM location 0 in the left top corner of the LCD
	
****************************************************************************/
unsigned char Write_String1(void)
{	
	char temp0[2];
	char temp1[sizeof(String1)];
	char temp2[2];	
	char state,i,adr;
	tx_type tx_frame[4];

	state = SUCCESS;
	adr = 0;
	
//Struct # 0 in the frame	
	tx_frame[0].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[0].size = 2;					//Number of bytes to send
	tx_frame[0].data_ptr = temp0;			//Set the pointer to temp array	0

	tx_frame[0].data_ptr[0] = FUNC_SET;		//Enter function setting
	tx_frame[0].data_ptr[1] = DDRAM_ADR + adr;//LCD adr counter set to "adr"

//Struct # 1 in the frame														
	tx_frame[1].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[1].size = (sizeof(String1));	//Number of bytes to send
	tx_frame[1].data_ptr = temp1;			//Set the pointer to temp array	1	

	tx_frame[1].data_ptr[0] = DDRAM_WR;		//Enter function setting

	//Copy the text string into a temp array
	memcpy_P(tx_frame[1].data_ptr+1,String1,(tx_frame[1].size-1));
				
	for(i=1;i<(tx_frame[1].size);i++)		//Add a ASCII offset
		tx_frame[1].data_ptr[i] += ASCII_OFFSET;
	
//Struct # 2 in the frame	
	tx_frame[2].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[2].size = 2;					//Number of bytes to send
	tx_frame[2].data_ptr = temp2;			//Set the pointer to temp array	2

	tx_frame[2].data_ptr[0] = RS_CONT;		//Select LCD Control Byte
	tx_frame[2].data_ptr[1] = CLR_ADR;		//Set the LCD DDRAM address 
											//counter to 0 and return the
											//shifted display to it's 
											//original position

	tx_frame[3].slave_adr = OWN_ADR;		//Set to OWN_ADR to end TWI frame

	state =Send_to_TWI(tx_frame);			//Call the Master TWI driver with
											//a pointer to the first struct 
											//in the package

	return state;							//If error occured during the TWI
											//comunication, return TWSR
											//If  no error, return SUCCESS

}    

/****************************************************************************
	Function : unsigned char Read_LCD_controlreg(void)						
	Read the LCD Read Control byte 
 		bit 0 -6 DDRAM Address Counter (AC)
 		bit 7 Busy Flag (BF, Indicating internal operation is being performed)

****************************************************************************/
unsigned char Read_LCD_controlreg(void)
{	
	char state;
	char temp;
	tx_type tx_frame[3];

	state = SUCCESS;
	
	tx_frame[0].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[0].size = 1;					//Number of bytes to send
	tx_frame[0].data_ptr = &temp;			//Set the pointer to temp's address
	tx_frame[0].data_ptr[0] = READ_CONT_REG;//Access LCD Busy flag and AC

	tx_frame[1].slave_adr = LCD_ADR+R;		//Init a Master Read
	tx_frame[1].size = 1;					//Number of bytes to read
	tx_frame[1].data_ptr = &temp;			//Set the pointer to temp's address

	tx_frame[2].slave_adr = OWN_ADR;		//Set to OWN_ADR to end TWI frame

	state =Send_to_TWI(tx_frame);			//Call the Master TWI driver with
											//a pointer to the first struct 
											//in the package
	
	if(state != SUCCESS)					//If error occured during the TWI
		return state;						//comunication, return TWSR
	return temp;							//If  no error, return the Read
											//Control byte from the LCD
}

/****************************************************************************
	Function : unsigned char Shift_left(void)						
	Shift the displayed part of DDRAM once to the left to make a scroll text
	
****************************************************************************/
unsigned char Shift_left(void)						
{
	char temp0[2];
	char state;
	tx_type tx_frame[2];

	state = SUCCESS;
	
	tx_frame[0].slave_adr = LCD_ADR+W;		//Slave adr + Write
	tx_frame[0].size = 2;					//Number of bytes to send
	tx_frame[0].data_ptr = temp0;			//Set the pointer to temp array	0
	tx_frame[0].data_ptr[0] = FUNC_SET;		//Enter function setting
	tx_frame[0].data_ptr[1] = SHIFT_LEFT;	//Shift the LCDtext one time to 
											//the left
	tx_frame[1].slave_adr = OWN_ADR;		//Set to OWN_ADR to end TWI frame

	state =Send_to_TWI(tx_frame);			//Call the Master TWI driver with
											//a pointer to the first struct 
											//in the package
	return state;
}    


/****************************************************************************
Function : void Delay1mS(int number_of_mS)
One millisecond delay loop @ 4MHz. Parameter "number_of_mS" tells how may
times to run the loop. i.e. number_of_mS = 3000 -> 3 sec delay
*****************************************************************************/
void Delay1mS(int number_of_mS)
{
	int	i;
	
	while(number_of_mS--)
	{
		i = 0x1F3;							//Approximately 1mS @ 4MHZ	
		while(i--)
			;
	}		
}

/****************************************************************************
	Function : void C_task main (void)
	Main function for controlling the LCD. Sets up the LCD, write a string to
	it and shift it left as a scrolling text.

****************************************************************************/
void C_task main (void)
{
	char state;

	setup();   								//Setup the AVR

	while(Read_LCD_controlreg() & BUSY_FLAG)//Wait until Busyflag is cleared
		;
	do
	{
		state = Init_LCD();					//Setup the LCD
	}while(state != SUCCESS);	

	while(Read_LCD_controlreg() & BUSY_FLAG)//Wait until Busyflag is cleared
		;
	do
	{
		state = Clear_LCD();    			//Write blanks to the entire 
	}while(state != SUCCESS);				//LCD DDRAM

		
	while(Read_LCD_controlreg() & BUSY_FLAG)//Wait until Busyflag is cleared
		;			
	do
	{
		state =  Write_String1();    		//Write String1 to the display
	}while(state != SUCCESS);	

//	Delay1mS(500);							//1/2 second delay	

	for(;;)									//Make a scroll text
	{
		/*A eternal loop with a 1/2 second delay. Waits until LCD busy flag 
		is cleared and shift the display text left to make the text scroll*/
		Delay1mS(500);
		while(Read_LCD_controlreg() & BUSY_FLAG)
			;								
		Shift_left();						
	}
}    

