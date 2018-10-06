/**
	\file
	\brief
		This is the source file for the PIT device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the PIT peripheral.
	\authors  Margarita Jauregui, Alondra Macias
	\date	30/09/2018
 */


#include "PIT.h"
#include "DataTypeDefinitions.h"


static uint8_t g_pit_intr_flag_0 = FALSE;
static uint8_t g_pit_intr_flag_1 = FALSE;
static uint8_t g_pit_intr_flag_2 = FALSE;
static uint8_t g_pit_intr_flag_3 = FALSE;
static uint32_t g_read = 0;


void PIT0_IRQHandler()
{
	volatile uint32_t dummyRead;

	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[0].TCTRL;	//read control register for clear PIT flag, this is silicon bug

	PIT->CHANNEL[PIT_0].TCTRL &= ~(PIT_TCTRL_TIE_MASK);//enables PIT timer interrupt
	PIT->CHANNEL[0].TCTRL &= ~(PIT_TCTRL_TEN_MASK);//enables timer0

	g_pit_intr_flag_0 = TRUE;
}


void PIT1_IRQHandler()
{
	volatile uint32_t dummyRead;

	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[1].TCTRL;	//read control register for clear PIT flag, this is silicon bug

	PIT->CHANNEL[PIT_1].TCTRL &= ~(PIT_TCTRL_TIE_MASK);//enables PIT timer interrupt
	PIT->CHANNEL[1].TCTRL &= ~(PIT_TCTRL_TEN_MASK);//enables timer0

	g_pit_intr_flag_1 = TRUE;
}

void PIT2_IRQHandler()
{
	volatile uint32_t dummyRead;

	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[2].TCTRL;	//read control register for clear PIT flag, this is silicon bug

	PIT->CHANNEL[PIT_2].TCTRL &= ~(PIT_TCTRL_TIE_MASK);//enables PIT timer interrupt
	PIT->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);//enables timer0
	g_read = GPIO_read_port(GPIO_D);
	g_pit_intr_flag_2 = TRUE;
}


void PIT3_IRQHandler()
{
	volatile uint32_t dummyRead;

	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
	dummyRead = PIT->CHANNEL[2].TCTRL;	//read control register for clear PIT flag, this is silicon bug

	PIT->CHANNEL[PIT_2].TCTRL &= ~(PIT_TCTRL_TIE_MASK);//enables PIT timer interrupt
	PIT->CHANNEL[2].TCTRL &= ~(PIT_TCTRL_TEN_MASK);//enables timer0
	g_read = GPIO_read_port(GPIO_D);
	g_pit_intr_flag_3 = TRUE;
}

void PIT_delay(PIT_timer_t pit_timer,My_float_pit_t system_clock , My_float_pit_t delay)
{
	uint32_t LDVAL = 0;
	My_float_pit_t clock_period = 0.0F;
	system_clock = system_clock /2;
	clock_period = (1/system_clock);
	LDVAL = (uint32_t)(delay / clock_period);
	LDVAL = LDVAL - 1;

	PIT->MCR  &= ~PIT_MCR_MDIS_MASK;//turns on PIT
	PIT->MCR  |= PIT_MCR_FRZ_MASK;

	switch(pit_timer)
	{
	case PIT_0:
		PIT->CHANNEL[0].LDVAL = LDVAL;
		PIT_enable_interrupt(PIT_0);
		break;
	case PIT_1:
		PIT->CHANNEL[1].LDVAL = LDVAL;
		PIT_enable_interrupt(PIT_1);
		break;
	case PIT_2:
			PIT->CHANNEL[2].LDVAL = LDVAL;
			PIT_enable_interrupt(PIT_2);
			break;
	case PIT_3:
			PIT->CHANNEL[3].LDVAL = LDVAL;
			PIT_enable_interrupt(PIT_3);
			break;

	default:
		PIT->CHANNEL[0].LDVAL = LDVAL;
		PIT_enable_interrupt(PIT_0);
		break;
	}


}

void PIT_enable(void)
{
	PIT->MCR  |= PIT_MCR_FRZ_MASK;
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; /* Enable PIT*/
}

 void PIT_enable_interrupt(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;//enables PIT timer interrupt
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TEN_MASK;//enables timer0
}

void PIT_clock_gating(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT0_get_interrupt_glag_status(void)
{
	return(g_pit_intr_flag_0);
}

void PIT0_clear_interrupt_flag(void)
{
	g_pit_intr_flag_0 = FALSE;
}


uint8_t PIT1_get_interrupt_glag_status(void)
{
	return(g_pit_intr_flag_1);
}

void PIT1_clear_interrupt_flag(void)
{
	g_pit_intr_flag_1 = FALSE;
}


uint8_t PIT2_get_interrupt_glag_status(void)
{
	return(g_pit_intr_flag_2);
}

void PIT2_clear_interrupt_flag(void)
{
	g_pit_intr_flag_2 = FALSE;
}

uint8_t PIT3_get_interrupt_glag_status(void)
{
	return(g_pit_intr_flag_3);
}

void PIT3_clear_interrupt_flag(void)
{
	g_pit_intr_flag_3 = FALSE;
}

uint32_t get_read_data(void)
{
	return g_read;
}

void PIT_stop(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL &= ~(PIT_TCTRL_TEN_MASK);//disables timer
}


