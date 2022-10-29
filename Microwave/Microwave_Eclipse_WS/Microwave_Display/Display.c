/******************************************************************************
 *
 * Module: Microwave Display & Control
 *
 * Description: Source file for Microwave Display & Control
 *
 *******************************************************************************/

#include "display.h"
#include <avr/interrupt.h>
#include "Timers.h"
#include "gpio.h"
#include "std_types.h"
#include "Buzzer.h"
#include "dc-motor.h"
#include "uart.h"
#include "Internal_Light.h"
#include "common_macros.h"

/*******************************************************************************
 *                                Global Variables                             *
 *******************************************************************************/

/* Variable to check state microwave is working or not */
volatile uint8 check_start = INTIAL_VALUE;

/* Display Digits Second & Minutes */
volatile uint8 SEC_1 = INTIAL_VALUE;
volatile uint8 SEC_2 = INTIAL_VALUE;
volatile uint8 MIN_1 = INTIAL_VALUE;
volatile uint8 MIN_2 = INTIAL_VALUE;

/*******************************************************************************
 *                                Static Variables                             *
 *******************************************************************************/

/* Variable to receive a digit from Keypad */
static uint8 rx_temp;

/* Call back pointer to function */
static void (*g_callBackPtr_INT1)	(void) 	= NULL_PTR;

/* Variable to set microwave state to STOP or PAUSE */
static volatile uint8 STOP_PAUSE_temp = INTIAL_VALUE;

/*
 * Setting Timer Configurations with:
 * - Selecting Timer1
 * - Set Pre-Scaler to 1024
 * - Select CTC Mode
 */
static Timer_ConfigType Timer_Config = {Timer_1, Timer_Prescaler_64, Timer1_CompareA_Mode, 0, 0, 0, 0, 0};


/*******************************************************************************
 *                              Functions Definitions                          *
 *******************************************************************************/

/*
 * Description :
 * External Interrupt Handler
 */
ISR(INT1_vect)
{
	/* Call the call back function if it init-ate with function address */
	if(g_callBackPtr_INT1 != NULL_PTR)
	{
		(*g_callBackPtr_INT1)();
	}
}

/*
 * Description :
 * Function responsible for init-ializing the display
 */
void display_init(void)
{
	GPIO_setupPinDirection(PORTA_ID, PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTA_ID, PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTA_ID, PIN2_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTA_ID, PIN3_ID, PIN_OUTPUT);
	GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW);
	GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_LOW);
	GPIO_writePin(PORTA_ID, PIN2_ID, LOGIC_LOW);
	GPIO_writePin(PORTA_ID, PIN3_ID, LOGIC_LOW);

	GPIO_setupPinDirection(PORTB_ID, PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN2_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_HIGH);
	GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_HIGH);
	GPIO_writePin(PORTB_ID, PIN2_ID, LOGIC_HIGH);
	GPIO_writePin(PORTB_ID, PIN3_ID, LOGIC_HIGH);
}

/*
 * Description :
 * Function responsible for setting display digits
 */
void set_digits(void)
{
	/* Send IAM_READY byte to Microwave Keypad to ask it to send the digits */
	UART_sendByte(IAM_READY);

	/* waiting to receive  digit location */
	rx_temp = UART_recieveByte();

	/* after receiving digit location, waiting to receive digit number */
	switch(rx_temp)
	{
	case 1:
		/* Send FINISH byte to Microwave Keypad to ask it to send the digits */
		UART_sendByte(FINISH);
		SEC_1 = UART_recieveByte();
		break;

	case 2:
		SEC_2 = SEC_1;
		UART_sendByte(FINISH);
		SEC_1 = UART_recieveByte();
		break;

	case 3:
		MIN_1 = SEC_2;
		SEC_2 = SEC_1;
		UART_sendByte(FINISH);
		SEC_1 = UART_recieveByte();
		break;

	case 4:
		MIN_2 = MIN_1;
		MIN_1 = SEC_2;
		SEC_2 = SEC_1;
		UART_sendByte(FINISH);
		SEC_1 = UART_recieveByte();
		break;
	}
}

/*
 * Description :
 * Function responsible for turning microwave ON
 */
void start(void)
{
	/* Checking DOOR is open or close and enter to turning ON microwave after insuring DOOR is closed */
	if(GPIO_readPin(PORTC_ID, PIN0_ID))
	{
		/* Checking all digits is zero or not */
		if( (SEC_1 == INTIAL_VALUE) && (SEC_2 == INTIAL_VALUE) && (MIN_1 == INTIAL_VALUE) && (MIN_2 == INTIAL_VALUE) )
		{
			/* In case all digits is zero microwave will not working and
			 * send FINISH byte to Keypad to receive another digits
			 */
			UART_sendByte(FINISH);
		}
		else
		{
			/* Init-ialize Timer call back function */
			Timer1_setCallBack(countdown);

			/* Timer delay for 1 Second with Compare Interrupt */
			Timer_Config.T1_Comp_Value = 15625;

			/* Timer Init-ialization with structure */
			Timer_init(&Timer_Config);

			/* Turning Internal light ON */
			internalLight_on();

			/* Switch on DC Motor */
			DcMotor_Rotate(CW);

			/* set the variable */
			check_start = FIRST_DIG;

			/* reset the variable */
			STOP_PAUSE_temp = INTIAL_VALUE;
		}
	}
	/* In case door is open microwave will not turning ON */
	else
	{
		UART_sendByte(FINISH);
	}
}

/*
 * Description :
 * Function responsible for stop microwave
 */
void stop(void)
{
	pause();

	SEC_1 = INTIAL_VALUE;
	SEC_2 = INTIAL_VALUE;
	MIN_1 = INTIAL_VALUE;
	MIN_2 = INTIAL_VALUE;
}

/*
 * Description :
 * Function responsible for pause microwave
 */
void pause(void)
{
	/* Disable Timer1 CompareA Interrupt */
	CLEAR_BIT(TIMSK, OCIE1A);

	/* Stop the DC-Motor */
	DcMotor_Rotate(ST);

	/* Turning Internal light OFF */
	internalLight_off();

	/* reset the variable */
	check_start = INTIAL_VALUE;

	/* send FINISH byte to Keypad to receive another digits */
	UART_sendByte(FINISH);
}

/*
 * Description :
 * Function responsible for setting all digits to ZERO
 */
void ZERO_DIG(void)
{
	/* Reset all digits to ZERO */
	SEC_1 = INTIAL_VALUE;
	SEC_2 = INTIAL_VALUE;
	MIN_1 = INTIAL_VALUE;
	MIN_2 = INTIAL_VALUE;

	/* Send ZERO_DIG byte to receive digits in 1st display location */
	UART_sendByte(DIG_ZEROS);
}

/*
 * Description :
 * Function responsible for counting down the timer
 */
void countdown(void)
{
	/* Checking all digits is reach to zero or not */
	if( (SEC_1 == INTIAL_VALUE) && (SEC_2 == INTIAL_VALUE) && (MIN_1 == INTIAL_VALUE) && (MIN_2 == INTIAL_VALUE) )
	{
		/* Stop the microwave */
		stop();

		/* Turning ON buzzer after finishing */
		buzzer_on();

		/* Init-ialize Timer call back function */
		Timer1_setCallBack(after_finish);

		/* Timer delay for 2 Second with Compare Interrupt */
		Timer_Config.T1_Comp_Value = 30000;

		/* Timer Init-ialization with structure */
		Timer_init(&Timer_Config);
	}

	/* setting all digits to count down */
	else if((SEC_1 & 0x0F) == INTIAL_VALUE)
	{
		SEC_1 = 9;
		if((SEC_2 & 0x0F) == INTIAL_VALUE)
		{
			SEC_2 = 5;
			if((MIN_1 & 0x0F) == INTIAL_VALUE)
			{
				MIN_1 = 9;
				if((MIN_2 & 0x0F) != INTIAL_VALUE)
				{
					MIN_2--;
				}
			}
			else
			{
				MIN_1--;
			}
		}
		else
		{
			SEC_2--;
		}
	}
	else
	{
		SEC_1--;
	}
}

/*
 * Description :
 * Function responsible for init-alizing External Interrupt
 */
void interrupts_init(void)
{
	/* Set Pin direction as input */
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID, PIN_INPUT);

	/* Enable External INT1 */
	SET_BIT(GICR, INT1);

	/* Set INT1 to interrupt at Rising Edge
	 * ISC10 = 1
	 * ISC11 = 1
	 */
	MCUCR |= 0X0C;
}

/*
 * Description :
 * Function responsible for stop or pause microwave after turning it ON or reset all digits in case
 * microwave isn't ON
 */
void stop_pause(void)
{
	/* In case microwave is ON set it pause or stop */
	if(check_start)
	{
		STOP_PAUSE_temp++;

		if(STOP_PAUSE_temp == 1)
		{
			pause();
		}
		else if(STOP_PAUSE_temp == 2)
		{
			ZERO_DIG();
			STOP_PAUSE_temp = INTIAL_VALUE;
		}
	}

	/* In case microwave is OFF, reset all digits */
	else
	{
		ZERO_DIG();
	}
}

/*
 * Description :
 * Function responsible for stop alarm after microwave finishing
 */
void after_finish(void)
{
	/* Disable Timer1 CompareA Interrupt */
	CLEAR_BIT(TIMSK, OCIE1A);

	/* Stop buzzer */
	buzzer_off();

	/* send FINISH byte to Keypad to receive new digits */
	UART_sendByte(FINISH);
}

/*
 * Description :
 * Function responsible for setting the call back function
 */
void INT1_setCallBack	(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_INT1 = a_ptr;
}
