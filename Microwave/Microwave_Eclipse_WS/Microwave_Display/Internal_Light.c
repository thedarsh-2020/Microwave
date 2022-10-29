/******************************************************************************
 *
 * Module: Microwave Internal Light
 *
 * Description: Source file for Microwave Internal Light
 *
 *******************************************************************************/

#include "Internal_Light.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initiate Internal Light pin direction and its initial state
 */
void internalLight_init(void)
{
	/* setup pin direction as output */
	GPIO_setupPinDirection(PORTC_ID, PIN3_ID, PIN_OUTPUT);

	/* set Internal Light initial state as turned off */
	GPIO_writePin(PORTC_ID, PIN3_ID, LOGIC_LOW);
}


/*
 * Description :
 * Function responsible for turn on Internal Light
 */
void internalLight_on(void)
{
	GPIO_writePin(PORTC_ID, PIN3_ID, LOGIC_HIGH);
}


/*
 * Description :
 * Function responsible for turn off Internal Light
 */
void internalLight_off(void)
{
	GPIO_writePin(PORTC_ID, PIN3_ID, LOGIC_LOW);
}
