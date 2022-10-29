/******************************************************************************
 *
 * Module: Microwave KeyPad
 *
 * Description: Header file for Microwave KeyPad
 *
 *******************************************************************************/

#ifndef MICRO_KEYPAD_H_
#define MICRO_KEYPAD_H_

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
 * Function responsible for init-alizing External Interrupt
 */
void interrupts_init	(void);

/*
 * Description :
 * Function responsible for sending digit inputs from client to Display it
 */
void set_digits			(uint8 dig);

/*
 * Description :
 * Function responsible for sending start signal to start Motor Rotate
 */
void start				(void);

/*
 * Description :
 * Function responsible for setting the call back function
 */
void INT0_setCallBack	(void(*a_ptr)(void));

#endif /* MICRO_KEYPAD_H_ */
