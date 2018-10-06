/*
 * Switches.h
 *
 *  Created on: Sep 24, 2018
 *      Author: marga
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "GPIO.h"
#include "NVIC.h"

/*Value used to manipulate corresponding pin for SW3 on port C*/
#define SW3_PORT_CONFIG 0x00000010
/*Value used to manipulate corresponding pin for SW2 on port A*/
#define SW2_PORT_PORT 0x00000040



#define SW2_port GPIO_C
#define SW3_port GPIO_A
/*Constant that represent the pin number for SW2*/
#define SW2_PIN 6
/*Constant that represent the pin number for SW2*3*/
#define SW3_PIN 4
/*! These constants are used to select between SW2 and SW3*/
typedef enum{SW2,
			SW3
}SWITCH_name;



/*!
 \brief
 	 	 This function configures both switches as GPIO inputs and enables them to interrupt on falling edge
 	 \return void
 */
void SW_setup(void);

/*!
 \brief
 	 	 Getter function of SW2 interruption flag
 	 \return uint8_t
 */
uint8_t sw2_get_intr_flag(void);


/*!
 \brief
 	 	 Getter function of SW3 interruption flag
 	 \return uint8_t
 */
uint8_t sw3_get_intr_flag(void);

/*!
 \brief
 	 	 Getter function of SW2 interruption flag
 	 \return void
 */
void sw2_clear_intr_flag(void);


/*!
 \brief
 	 	 Getter function of SW3 interruption flag
 	 \return void
 */
void sw3_clear_intr_flag(void);
/*!
 \brief
 	 	This function configures the priority of the interruption of sw2
 	 \return void
 */

void sw2_enable_interrupt(void);
/*!
 \brief
 	 	This function configures the priority of the interruption of sw3
 	 \return void
 */
void sw3_enable_interrupt(void);


#endif /* SWITCHES_H_ */
