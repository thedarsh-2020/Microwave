/******************************************************************************
 *
 * Module: Microwave Internal Light
 *
 * Description: Header file for Microwave Internal Light
 *
 *******************************************************************************/

#ifndef INTERNAL_LIGHT_H_
#define INTERNAL_LIGHT_H_

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initiate Internal Light pin direction and its initial state
 */
void internalLight_init(void);

/*
 * Description :
 * Function responsible for turn on Internal Light
 */
void internalLight_on(void);

/*
 * Description :
 * Function responsible for turn off Internal Light
 */
void internalLight_off(void);


#endif /* INTERNAL_LIGHT_H_ */
