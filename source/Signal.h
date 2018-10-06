/*
 * Signal.h
 *
 *  Created on: Sep 15, 2018
 *      Author: AlondraItzel
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "DataTypeDefinitions.h"
#include "DAC.h"
#include "NVIC.h"
#include "GPIO.h"
#include "PIT.h"

#define CLOCK_RATE 21000000
#define SIGNAL_PERIOD
#define SIGNAL_PORT GPIO_C
#define SIGNAL_PIN 16
#define NEXT_STATE 1
#define Low_MASK 0x00FF
#define High_MASK 0x00F
#define DELAY1 .2 //square-
#define DELAY2 .002 //triangle
#define DELAY3 .004 //sen-

typedef enum
{	OFF,
	SQUARE,
	SIN,
	TRIANGLE
}signal_form_t;



/*!
 \brief This function configures the Dac and the priority of the interruption
 \param[in] void
 \return void
 */
void Signal_enable(void);
/*!
 	\brief evaluate if the flags are unlocked to give a signal and the sw3 interruption to change the state
 	\param[in] int
 	\return void
 */
void Signal(void);
/*!
 	\brief This function return the type of signal that is needed
 	\param[in] int
 	\return void
 */
void type_signal(signal_form_t type);
/*!
 	\brief This function configures to create a sin wave
 	\param[in] void
 	\return void
 */
void SIGNAL_sin(void);

/*!
 	\brief This function configures to create a square wave
 	\param[in] void
 	\return void
 */
void SIGNAL_square(void);


/*!
 	\brief This function configures to create a triangle wave
 	\param[in] void
 	\return void
 */
void SIGNAL_triangle(void);
/*!
 	\brief This function turns OFF the signal
 	\param[in] void
 	\return void
 */

void SIGNAL_off(void);

#endif /* SIGNAL_H_ */
