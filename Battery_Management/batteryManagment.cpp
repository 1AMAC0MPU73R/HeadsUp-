/*
 * batteryManagment.cpp
 *
 * Created: 3/15/2016 8:02:14 PM
 *  Author: Jack
 */ 

#define F_CPU	8000000UL				// 8 MHz

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//#include "lcd.h"

#define ZERO_EXTERNAL_COUNTER_INPUT_PORT		PORTB0
#define ONE_EXTERNAL_COUNTER_INPUT_PORT			PORTB1
#define ANALOG_COMPATATOR_NEG_INPUT_PORT		PORTB2
#define ANALOG_COMPARATOR_POS_INPUT_PORT		PORTB3
#define EXTERNAL_INTERRUPT_ONE_INPUT_PORT		PORTD3

#define External_Counter_input_pin			PINB0
#define Analog_Comparator_Negative_Pin		PINB2

typedef enum{
	CHARGE_OFF = 0,
	CHARGE_SLOW,		// 100mA (10K)
	CHARGE_MID,			// 370mA (2K7)
	CHARGE_FAST,		// 470mA (2K7 || 10K = 2.126K)
}charge_t;

static uint16_t batteryVoltage(void);
static void setChargeRate(charge_t);

/* Get the voltage of the battery */
static uint16_t batteryVoltage()
{
	/* Turn on battery voltage divider */
	PORTB |= (1 << ANALOG_COMPARATOR_POS_INPUT_PORT);
	
	/* Wait a little bit for things to turn on */
	_delay_us(200);
	
	/* ADC - Start */
	ADCSRA |= (1 << ADSC);	// start conversion
	
	/* Wait for ADC to complete */
	loop_until_bit_is_clear(ADCSRA, ADSC);
	
	/* Turn off battery voltage divider */
	PORTB &= ~(1 << ANALOG_COMPATATOR_NEG_INPUT_PORT);
	
	return ADC;
	
}

static void setChargeRate(charge_t charge)
{
	switch(charge) {
		case CHARGE_OFF:
		/* Both transistors off */
			PORTB &= ~(1 << ZERO_EXTERNAL_COUNTER_INPUT_PORT);
			PORTB &= ~(1 << ONE_EXTERNAL_COUNTER_INPUT_PORT);
			break;
		case CHARGE_SLOW:
			PORTB &= ~(1 << ZERO_EXTERNAL_COUNTER_INPUT_PORT);
			PORTB |= (1 << ONE_EXTERNAL_COUNTER_INPUT_PORT);
			break;
		case CHARGE_MID:
			PORTB |= (1 << ZERO_EXTERNAL_COUNTER_INPUT_PORT);
			PORTB &= ~(1 << ONE_EXTERNAL_COUNTER_INPUT_PORT);
			break;
		case CHARGE_FAST:
			PORTB |= (1 << ZERO_EXTERNAL_COUNTER_INPUT_PORT);
			PORTB |= (1 << ONE_EXTERNAL_COUNTER_INPUT_PORT);
			break;
		default:
			break;
	}
}

int main(void)
{
	/* Charge control pins as output */
	DDRB |= (1 << ZERO_EXTERNAL_COUNTER_INPUT_PORT);
	DDRB |= (1 << ONE_EXTERNAL_COUNTER_INPUT_PORT);
	
	/* Turn off charging */
	setChargeRate(CHARGE_OFF);
	
	/* Battery voltage divider control pin as output */
	DDRB |= (1 << ANALOG_COMPARATOR_POS_INPUT_PORT);
	
	/* Charge state pin as input with pull-up */
	PORTD |= (1 << EXTERNAL_INTERRUPT_ONE_INPUT_PORT);
	
	/* Dont need to do anything with USB power sense pin since
	*  default is input with pull-up disabled (R3 is a pull-down)
	*/
	
	/* ADC - Set Up */
	/* Select 1.1 V Internal Reference with external capacitor at AREF pin */
	/* MUX1 and MUX0 picks ADC3 pin */
	ADMUX |= (1 << REFS1) | (1 << MUX1) | (1 << MUX0);		// NEED EXTN Cap at AREFF
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// Enable ADC prescaler ser to 64 (125KHz @ 8 MHz)
	DIDR0 |= (1 << ADC3D);									// Disable digital input buffer on ADC3 pin
	
	while(1) {
		/* Get battery voltage */
		uint16_t voltage = batteryVoltage();
		
		/* Convert to mV */
		voltage = ( (uint32_t)voltage * 4200) / 1023;
		
		/* Get charge state */
		bool isCharging = bit_is_clear(PINB, External_Counter_input_pin);
				
		/* Get USB state */
		bool usbPower = bit_is_set(PINB, Analog_Comparator_Negative_Pin);
		
		/* Set Charge
		*  Battery power will be wasted if transistors are not turned off when
		*  not charging
		*/
		if(usbPower) {
			setChargeRate(CHARGE_SLOW);
			//print("USB Charge slow",LINE_BOTTOM);
		} else {
			setChargeRate(CHARGE_OFF);
			//print("USB not charge",LINE_BOTTOM);
		}
	
		_delay_ms(1000);
	}
}