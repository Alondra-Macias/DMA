/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\authors  Margarita Jauregui, Alondra Macias
	\date	30/09/2018
 */


#include "MK64F12.h"
#include "GPIO.h"


static BooleanType sw3_intr_flag;
static BooleanType sw2_intr_flag;
static BooleanType keyboard_intr_flag;




/*ISR for interruption on PORTD*/
void PORTD_IRQHandler()
{

	GPIO_clear_interrupt(GPIO_D);
	PIT_delay(PIT_2, SYSTEM_CLOCK, READ_DELAY);

}


BooleanType GPIO_get_sw2_intr()
{
	return sw2_intr_flag;
}

BooleanType GPIO_get_sw3_intr()
{
	return sw3_intr_flag;
}

BooleanType GPIO_get_kb_intr()
{
	keyboard_intr_flag = GPIO_read_pin(GPIO_D, DATA_AVAILABLE_PIN);
	return keyboard_intr_flag;
}

void GPIO_clear_sw2_intr()
{
	sw2_intr_flag = FALSE;
}

void GPIO_clear_sw3_intr()
{
	sw3_intr_flag = FALSE;
}

void GPIO_clear_kb_intr()
{
	keyboard_intr_flag = FALSE;
}

void GPIO_clear_interrupt(gpio_port_name_t portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}
uint8_t GPIO_clock_gating(gpio_port_name_t portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t portName, uint8_t pin,const gpio_pin_control_register_t*  pinControlRegister)
{
	switch(portName)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pinControlRegister;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_write_port(gpio_port_name_t portName, uint32_t data)
{
	switch (portName)
	{
		case (GPIO_A):
			GPIOA->PDOR = data;
			break;
		case (GPIO_B):
			GPIOB->PDOR = data;
			break;
		case (GPIO_C):
			GPIOC->PDOR = data;
			break;
		case (GPIO_D):
			GPIOD->PDOR = data;
			break;
		case (GPIO_E):
			GPIOE->PDOR = data;
			break;
		default:
			break;
	}
}

uint32_t GPIO_read_port(gpio_port_name_t portName)
{
	switch (portName)
	{
		case (GPIO_A):
			return (GPIOA->PDIR);
			break;
		case (GPIO_B):
			return (GPIOB->PDIR);
			break;
		case (GPIO_C):
			return (GPIOC->PDIR);
			break;
		case (GPIO_D):
			return (GPIOD->PDIR);
			break;
		case (GPIO_E):
			return (GPIOE->PDIR);
			break;
		default:
			return (0);
			break;
	}
}


uint8_t GPIO_read_pin(gpio_port_name_t portName, uint8_t pin)
{

	uint8_t retValue = 0;

	switch (portName)
	{
		case (GPIO_A):
			retValue = (GPIOA->PDIR) >> pin;
			break;
		case (GPIO_B):
			retValue = (GPIOB->PDIR) >> pin;
			break;
		case (GPIO_C):
			retValue = (GPIOC->PDIR) >> pin;
			break;
		case (GPIO_D):
			retValue = (GPIOD->PDIR) >> pin;
			break;
		case (GPIO_E):
			retValue = (GPIOE->PDIR) >> pin;
			break;
		default:
			return (FALSE);
	}
	return retValue;
}

void GPIO_set_pin(gpio_port_name_t portName, uint8_t pin)
{
	uint32_t value = MASK_PIN_SELECTED;
	value <<= pin;
	switch (portName)
	{
		case (GPIO_A):
			GPIOA->PSOR |= value;
			break;
		case (GPIO_B):
			GPIOB->PSOR |= value;
			break;
		case (GPIO_C):
			GPIOC->PSOR |= value;
			break;
		case (GPIO_D):
			GPIOD->PSOR |= value;
			break;
		case (GPIO_E):
			GPIOE->PSOR |= value;
			break;
		default:
			break;
	}
}

void GPIO_clear_pin(gpio_port_name_t portName, uint8_t pin)
{
	uint32_t value = MASK_PIN_SELECTED;
	value <<= pin;
	switch (portName)
	{
		case (GPIO_A):
			GPIOA->PCOR |= value;
			break;
		case (GPIO_B):
			GPIOB->PCOR |= value;
			break;
		case (GPIO_C):
			GPIOC->PCOR |= value;
			break;
		case (GPIO_D):
			GPIOD->PCOR |= value;
			break;
		case (GPIO_E):
			GPIOE->PCOR |= value;
			break;
		default:
			break;
	}
}

void GPIO_toogle_pin(gpio_port_name_t portName, uint8_t pin)
{
	uint32_t value = MASK_PIN_SELECTED;
	value <<= pin;
	switch (portName)
	{
		case (GPIO_A):
			GPIOA->PTOR = value;
			break;
		case (GPIO_B):
			GPIOB->PTOR = value;
			break;
		case (GPIO_C):
			GPIOC->PTOR = value;
			break;
		case (GPIO_D):
			GPIOD->PTOR = value;
			break;
		case (GPIO_E):
			GPIOE->PTOR = value;
			break;
		default:
			break;
	}
}

void GPIO_data_direction_port(gpio_port_name_t portName ,uint32_t direction)
{
	switch(portName)
	{
		case (GPIO_A):
			GPIOA->PDDR = direction;
			break;
		case (GPIO_B):
			GPIOB->PDDR = direction;
			break;
		case (GPIO_C):
			GPIOC->PDDR = direction;
			break;
		case (GPIO_D):
			GPIOD->PDDR = direction;
			break;
		case (GPIO_E):
			GPIOE->PDDR = direction;
			break;
		default:
			break;
	}
}

void GPIO_data_direction_pin(gpio_port_name_t portName, uint8_t state, uint8_t pin)
{
	uint32_t value = state;
	value <<= pin;
	switch (portName)
	{
		case (GPIO_A):
			GPIOA->PDDR |= value;
			break;
		case (GPIO_B):
			GPIOB->PDDR |= value;
			break;
		case (GPIO_C):
			GPIOC->PDDR |= value;
			break;
		case (GPIO_D):
			GPIOD->PDDR |= value;
			break;
		case (GPIO_E):
			GPIOE->PDDR |= value;
			break;
		default:
			break;
	}
}



