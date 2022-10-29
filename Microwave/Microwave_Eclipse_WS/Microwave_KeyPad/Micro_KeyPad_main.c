/**************************************************************************************************
 *
 * Author      : Mustafa Muhammad
 *
 * Description : Main file for Microwave inputs KeyPad
 *
 **************************************************************************************************/

#include "keypad.h"
#include "uart.h"
#include "std_types.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "gpio.h"
#include "Micro_KeyPad.h"

/*******************************************************************************
 *                                Static Variables                             *
 *******************************************************************************/

/*
 * Variables Declaration
 * key : returned character from Keypad
 */
static volatile uint8 key;


int main(void)
{
	/* Init-ialize external interrupt pin as input & Enable the external interrupt */
	interrupts_init();

	/* Set the Call back function pointer */
	INT0_setCallBack(start);

	/* Init-ialize UART driver with:
	 * 8 Bit Data
	 * Baud-rate = 9600 Bits/Sec.
	 * One Stop Bit
	 * Disable Parity
	 */
	UART_ConfigType UART_Config = {Bits_8, BR_9600, Stop_1_Bit, Parity_Disable};
	UART_init(&UART_Config);

	/* Wait until Microwave Display is ready to receive */
	while(UART_recieveByte() != IAM_READY){}

	/* Enable UART RX Interrupt flag */
	SET_BIT(UCSRB, RXCIE);

	for(;;)
	{
		/* get pressed button from keypad */
		key = KEYPAD_getPressedKey();

		/* Check if the input between 0 to 9 and send to display it */
		switch(key)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			/* Function responsible for sending digit inputs from client to Display it */
			set_digits(key);
			break;
		}

		/* Press time */
		_delay_ms(500);
	}
}
