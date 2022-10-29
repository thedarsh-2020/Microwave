/**************************************************************************************************
 *
 * Author      : Mustafa Muhammad
 *
 * Description : Main file for Microwave (Display & Control)
 *
 **************************************************************************************************/

#include "uart.h"
#include "Timers.h"
#include <util/delay.h>
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"
#include "display.h"
#include "common_macros.h"
#include "Internal_Light.h"
#include "Buzzer.h"
#include "dc-motor.h"

/*******************************************************************************
 *                                Global Variables                             *
 *******************************************************************************/
extern volatile uint8 check_start;

extern volatile uint8 SEC_1;
extern volatile uint8 SEC_2;
extern volatile uint8 MIN_1;
extern volatile uint8 MIN_2;

/*******************************************************************************
 *                                Static Variables                             *
 *******************************************************************************/

/* Variable to receive a digit from Keypad */
volatile static uint8 received_temp = INTIAL_VALUE;

/*
 * Description :
 * UART Interrupt Handler
 */
ISR(USART_RXC_vect)
{
	received_temp = UDR;
}


int main(void)
{
	/* Init-ialize Buzzer Driver */
	buzzer_init();

	/* Init-ialize Motor Driver */
	DcMotor_Init();

	/* Init-ialize Internal Light Driver */
	internalLight_init();

	/* Init-ialize external interrupt pin as input & Enable the external interrupt */
	interrupts_init();

	/* Init-ialize DOOR pin */
	GPIO_setupPinDirection(PORTC_ID, PIN0_ID, PIN_INPUT);

	/* Init-ialize Digit displaying  */
	display_init();

	/* Init-ialize UART driver with:
	 * 8 Bit Data
	 * Baud-rate = 9600 Bits/Sec.
	 * One Stop Bit
	 * Disable Parity
	 */
	UART_ConfigType UART_Config = {Bits_8, BR_9600, Stop_1_Bit, Parity_Disable};
	UART_init(&UART_Config);


	/* Enable UART RX Interrupt flag */
	SET_BIT(UCSRB, RXCIE);

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Init-ialize External Interrupt function address */
	INT1_setCallBack(stop_pause);

	/* Send IAM_READY byte to Microwave Keypad to ask it to send the inputs */
	UART_sendByte(IAM_READY);

	for(;;)
	{
		/* According to received byte if it digit or start motor */
		switch(received_temp)
		{
		case DIGITS:
			/* Reset variable to not enter again unless received new byte */
			received_temp = INTIAL_VALUE;

			/* Disable UART RX Interrupt flag */
			CLEAR_BIT(UCSRB, RXCIE);

			/* Function responsible for setting display digits */
			set_digits();

			/* Enable UART RX Interrupt flag */
			SET_BIT(UCSRB, RXCIE);
			break;

		case START:
			/* Reset variable to not enter again unless received new byte */
			received_temp = INTIAL_VALUE;

			/* Function responsible for turning microwave ON */
			start();
			break;
		}

		/* Checking if door opened suddenly during microwave is ON */
		if(check_start)
		{
			/* If Door is opened suddenly and microwave is ON, it will stop */
			if(!GPIO_readPin(PORTC_ID, PIN0_ID))
			{
				/* Function responsible for pause microwave */
				pause();
			}
		}

		/* In case microwave is OFF and door is closed */
		else if(GPIO_readPin(PORTC_ID, PIN0_ID))
		{
			/* Turning Internal light OFF */
			internalLight_off();
		}

		/* In case microwave is OFF and door is open */
		else
		{
			/* Turning Internal light ON */
			internalLight_on();
		}


		/* Enable & Disable every 2 milli-second 7-Segments pins to show counter */
		PORTB &= 0XF0;
		SET_BIT(PORTB, 0);
		PORTA = (PORTA & 0XF0) | (SEC_1 & 0X0F);
		_delay_ms(2);

		PORTB &= 0XF0;
		SET_BIT(PORTB, 1);
		PORTA = (PORTA & 0XF0) | (SEC_2 & 0X0F);
		_delay_ms(2);

		PORTB &= 0XF0;
		SET_BIT(PORTB, 2);
		PORTA = (PORTA & 0XF0) | (MIN_1 & 0X0F);
		_delay_ms(2);

		PORTB &= 0XF0;
		SET_BIT(PORTB, 3);
		PORTA = (PORTA & 0XF0) | (MIN_2 & 0X0F);
		_delay_ms(2);
	}
}
