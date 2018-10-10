/**
	\file
	\brief
		This is the source file for the Switches Module.
		It contains all the implementation for configuration functions and runtime functions.
	\authors  Alondra Macias, Margarita Jauregui
	\date	30/09/2018
 */
#include "Switches.h"


/*
 \brief
 	 	 This function configures both switches as GPIO inputs and enables them to interrupt on falling edge
 	 \return void
 */
void SW_setup(void)
{
		/*Configuration to set switches as MUX-ALT1 (GPIO) with pull-up resistor enabled and interruption on falling edge*/
		gpio_pin_control_register_t sw_config = GPIO_MUX1 | GPIO_PE | GPIO_PS | INTR_FALLING_EDGE;

		/*Enables clock gating for the corresponding port for SW2*/
		GPIO_clock_gating(SW2_port);
		/*Enables clock gating for the corresponding port for SW3*/
		GPIO_clock_gating(SW3_port);
		/*Configures SW2 as ALT1, with Pull up resistor enabled and intr falling edge */
		GPIO_pin_control_register(SW2_port, SW2_PIN, &sw_config);
		/*Configures SW3 as ALT1, with Pull up resistor enabled and intr falling edge */
		GPIO_pin_control_register(SW3_port, SW3_PIN, &sw_config);

		/*configures Sw2 as input*/
		GPIO_data_direction_pin(SW2_port, GPIO_INPUT, SW2_PIN);
		/*configures Sw3 as input*/
		GPIO_data_direction_pin(SW3_port, GPIO_INPUT, SW3_PIN);

		NVIC_set_basepri_threshold(PRIORITY_12);// se activen desde el nivel del 0 al 12
		/*Enables and sets a particular interrupt and its priority*/
		NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5); //active y conf la prioridad del puerto c de nivel 5
		/*Enables and sets a particular interrupt and its priority*/
		NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_5); //mas importante que el 5

}




void sw2_enable_interrupt(void)
{
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_5);
}

void sw3_enable_interrupt(void)
{
	NVIC_enableInterruptAndPriotity(PORTA_IRQ, PRIORITY_5);
}


