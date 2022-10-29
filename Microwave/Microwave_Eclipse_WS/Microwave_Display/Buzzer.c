/******************************************************************************
 *
 * Module: Buzzer
 *
 * Description: Source file for Buzzer
 *
 *******************************************************************************/

#include "Buzzer.h"
#include "gpio.h"
#include "Timers.h"
#include <avr/io.h>
#include "common_macros.h"
#include "uart.h"
#include "Display.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initiate buzzer pin direction and its initial state
 */
void buzzer_init(void)
{
	/* setup pin direction as output */
	GPIO_setupPinDirection(PORTC_ID, PIN1_ID, PIN_OUTPUT);

	/* set buzzer initial state as turn it off */
	GPIO_writePin(PORTC_ID, PIN1_ID, LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for turn on buzzer
 */
void buzzer_on(void)
{
	GPIO_writePin(PORTC_ID, PIN1_ID, LOGIC_HIGH);
}

/*
 * Description :
 * Function responsible for turn off buzzer
 */
void buzzer_off(void)
{
	GPIO_writePin(PORTC_ID, PIN1_ID, LOGIC_LOW);
}
