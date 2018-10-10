/*
 * PIT.h
 *
 *  Created on: Sep 15, 2018
 *      Author: marga
 */

#ifndef PIT_H_
#define PIT_H_


#include "stdint.h"
#include "MK64F12.h"
#define MOTOR_PIT PIT_0
#define SIGNAL_PIT PIT_1
#define READ_DELAY 0.5F
#define SYSTEM_CLOCK 21000000
typedef float My_float_pit_t;
#include "GPIO.h"

/*! This enumerated constant are used to select the PIT to be used*/
typedef enum {PIT_0,PIT_1,PIT_2,PIT_3} PIT_timer_t;

/*!
 	 \brief	 The interruption of PIT0 IRQHandler to change the status flag
 	 \param[in] void
 	 \return void
 */
void PIT0_IRQHandler();
/*!
 	 \brief	 The interruption of PIT1 IRQHandler to change the status flag
 	 \param[in] void
 	 \return void
 */
void PIT1_IRQHandler();
/*!
 	 \brief	 The interruption of PIT2 IRQHandler to change the status flag
 	 \param[in] void
 	 \return void
 */
void PIT2_IRQHandler();
/*!
 	 \brief	 The interruption of PIT3 IRQHandler to change the status flag
 	 \param[in] void
 	 \return void
 */
void PIT3_IRQHandler();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configure the PIT to generate a delay base on the system clock.
 	 Internally it configures the clock gating and enables the PIT module.
 	 It is important to note that this strictly is not device driver since everything is
 	 contained in a single function, but in general you have to avoid this practices, this only
 	 for the propose of the homework

 	 \param[in]  portName Port to be configured.
 	 \return void
 */
void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay);

/********************************************************************************************/
/*!
 	 \brief	 This function enable the clock signal of the pit

 	 \param[in]  void.
 	 \return void
 */
void PIT_clock_gating(void);

/********************************************************************************************/
/*!
 	 \brief	It return the status of the interrupt flag. This flag is a variable created by the programmer.
 	 It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

 	 \param[in]  void.
 	 \return uint8_t flag status
 */
uint8_t PIT0_get_interrupt_glag_status(void);

/********************************************************************************************/
/*!
 	 \brief	Put the interruption flag of PIT0 in 0
 	 \param[in]  void.
 	 \return void
 */
void PIT0_clear_interrupt_flag(void);

/********************************************************************************************/
/*!
 	 \brief	It return the status of the interrupt flag. This flag is a variable created by the programmer.
 	 It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

 	 \param[in]  void.
 	 \return uint8_t flag status
 */
uint8_t PIT1_get_interrupt_glag_status(void);

/********************************************************************************************/
/*!
 	 \brief	Put the interruption flag of PIT1 in 0
 	 \param[in]  void.
 	 \return void
 */
void PIT1_clear_interrupt_flag(void);

/********************************************************************************************/
/*!
 	 \brief	enables the PIT
 	 \param[in]  void.
 	 \return void
 */
void PIT_enable(void);

/********************************************************************************************/
/*!
 	 \brief	It return clears the interrupt flag. This flag is a variable created by the programmer.
 	 It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

 	 \param[in]  void.
 	 \return uint8_t flag status
 */

uint8_t PIT2_get_interrupt_glag_status(void);
/********************************************************************************************/
/*!
 	 \brief	Put the interruption flag of PIT2 in 0
 	 \param[in]  void.
 	 \return void
 */

void PIT2_clear_interrupt_flag(void);
/********************************************************************************************/
/*!
 	 \brief	It return clears the interrupt flag. This flag is a variable created by the programmer.
 	 It is not the flag related with bit TIF in PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

 	 \param[in]  void.
 	 \return uint8_t flag status
 */

uint8_t PIT3_get_interrupt_glag_status(void);
/********************************************************************************************/
/*!
 	 \brief	Put the interruption flag of PIT3 in 0
 	 \param[in]  void.
 	 \return void
 */

void PIT3_clear_interrupt_flag(void);
/********************************************************************************************/
/*!
 	 \brief enables PIT timer interrupt
 	 \param[in]  PIT_timer_t the number of PIT to enable
 	 \return void
 */

 void PIT_enable_interrupt(PIT_timer_t pit);
 /********************************************************************************************/
 /*!
  	 \brief gives the status of g_read
  	 \param[in]  void
  	 \return  uint32_t return g_read
  */

 uint32_t get_read_data(void);
 /********************************************************************************************/
 /*!
  	 \brief It stop the number of PIT selected
  	 \param[in]  PIT_timer_t the number of PIT to stop
  	 \return void
  */

void PIT_stop(PIT_timer_t pit);

#endif /* PIT_H_ */
