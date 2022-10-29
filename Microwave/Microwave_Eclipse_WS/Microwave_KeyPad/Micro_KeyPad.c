/******************************************************************************
 *
 * Module: Microwave KeyPad
 *
 * Description: Source file for Microwave KeyPad
 *
 *******************************************************************************/

#include "Micro_KeyPad.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

/*******************************************************************************
 *                                Static Variables                             *
 *******************************************************************************/

/* Call back pointer to function */
static void (*g_callBackPtr_INT0)	(void) 	= NULL_PTR;

/* Variable to define digit location if it Second OR Minute */
static volatile uint8 FLAG_DIG = FIRST_DIG;

/*******************************************************************************
 *                              Functions Definitions                          *
 *******************************************************************************/

/*
 * Description :
 * USART Receiver Interrupt Handler
 */
ISR(USART_RXC_vect)
{
	/* Reset variable to start send from 1st digit */
	FLAG_DIG = FIRST_DIG;
}

/*
 * Description :
 * External Interrupt Handler
 */
ISR(INT0_vect)
{
	/* Call the Call Back function and check if i give it the func. address or not */
	if(g_callBackPtr_INT0 != NULL_PTR)
	{
		(*g_callBackPtr_INT0)();
	}
}

/*
 * Description :
 * Function responsible for init-alizing External Interrupt
 */
void interrupts_init(void)
{
	/* Set Pin direction as input */
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID, PIN_INPUT);

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Enable External INT0 */
	SET_BIT(GICR, INT0);

	/* Set INT0 to interrupt at Rising Edge
	 * ISC00 = 1
	 * ISC01 = 1
	 */
	MCUCR |= 0X03;
}

/*
 * Description :
 * Function responsible for sending digit inputs from client to Display it
 */
void set_digits(uint8 dig)
{
	/* Disable UART RX Interrupt flag */
	CLEAR_BIT(UCSRB, RXCIE);

	/* Ignore 0 if it's 1st digit */
	if(!((FLAG_DIG == FIRST_DIG) && (dig == INTIAL_VALUE)))
	{
		/* Check if number of digits exceed number of 7-seg */
		if(FLAG_DIG <= DIG_NUM)
		{
			/* send DIGITS byte to give a signal to be ready to send the inputs digits */
			UART_sendByte(DIGITS);

			/* Wait until Microwave Display is ready to receive */
			while(UART_recieveByte() != IAM_READY){}

			/* Sending Digit Location */
			UART_sendByte(FLAG_DIG);

			/* Wait until Microwave Display is ready to receive */
			while(UART_recieveByte() != FINISH){}

			/* Sending the input digit */
			UART_sendByte(dig);

			/* increment digit location */
			FLAG_DIG++;
		}
	}

	/* Enable UART RX Interrupt flag */
	SET_BIT(UCSRB, RXCIE);
}

/*
 * Description :
 * Function responsible for sending start signal to start Motor Rotate
 */
void start(void)
{
	/* Disable UART RX Interrupt flag */
	CLEAR_BIT(UCSRB, RXCIE);

	/* send START signal to start Motor rotate */
	UART_sendByte(START);

	/* Wait until Counter down is finish and reach to ZERO */
	while(UART_recieveByte() != FINISH){}

	/* set location begin again from 1st digit */
	FLAG_DIG = FIRST_DIG;

	/* Enable UART RX Interrupt flag */
	SET_BIT(UCSRB, RXCIE);
}

/*
 * Description :
 * Function responsible for setting the call back function
 */
void INT0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_INT0 = a_ptr;
}
