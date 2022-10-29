/******************************************************************************
 *
 * Module: Microwave Display & Control
 *
 * Description: Header file for Microwave Display & Control
 *
 *******************************************************************************/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define INTIAL_VALUE 	0
#define IAM_READY 		100
#define FIRST_DIG		1
#define DIG_NUM			4
#define DIG_ZEROS		11
#define START			12
#define PAUSE			13
#define STOP			14
#define FINISH			15
#define DIGITS			16

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for init-ializing the display
 */
void display_init		(void);

/*
 * Description :
 * Function responsible for setting display digits
 */
void set_digits			(void);

/*
 * Description :
 * Function responsible for turning microwave ON
 */
void start				(void);

/*
 * Description :
 * Function responsible for stop microwave
 */
void stop				(void);

/*
 * Description :
 * Function responsible for pause microwave
 */
void pause				(void);

/*
 * Description :
 * Function responsible for setting all digits to ZERO
 */
void ZERO_DIG			(void);

/*
 * Description :
 * Function responsible for counting down the timer
 */
void countdown			(void);

/*
 * Description :
 * Function responsible for init-alizing External Interrupt
 */
void interrupts_init	(void);

/*
 * Description :
 * Function responsible for stop or pause microwave after turning it ON or reset all digits in case
 * microwave isn't ON
 */
void stop_pause			(void);

/*
 * Description :
 * Function responsible for stop alarm after microwave finishing
 */
void after_finish		(void);

/*
 * Description :
 * Function responsible for setting the call back function
 */
void INT1_setCallBack	(void(*a_ptr)(void));

#endif /* DISPLAY_H_ */
