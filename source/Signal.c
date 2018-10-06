/**
	\file
	\brief
		This is the source file for the Signal Generator Module.
		It contains all the implementation for configuration functions and runtime functions.
	\authors  Alondra Macias, Margarita Jauregui
	\date	30/09/2018
 */
#include <stdio.h>
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "Signal.h"



static uint8_t g_pit_intr_status_1 = TRUE;
static uint8_t g_signal_state = 0;
BooleanType g_signal_enabled = TRUE; //acceder al proceso como tal
BooleanType sw3_interrupt = FALSE;
signal_form_t current_state1 = OFF;

uint16 square[2]={4095,0};
uint16 sen[100]={2048,2177,2305,2432,2557,2681,2802,2920,3035,3145,3252,3353,3450,3541,3626,3705,3777,3843,3901,3952,3996,4032,4060,4080,4092,4095,4092,4080,4060,4032,3996,3952,3901,3843,3777,3705,3626,3541,3450,3353,3252,3145,3035,2920,2802,2681,2557,2432,2305,2177,2048,1919,1791,1664,1539,1415,1294,1176,1061,951,844,743,646,555,470,391,319,253,195,144,100,64,36,16,4,0,4,16,36,64,100,144,195,253,319,391,470,555,646,743,844,951,1061,1176,1294,1415,1539,1664,1791,1919};
uint16 saw[200]={0,	41,	82,	123,	164,	205,	246,	287,	328,	369,	410,	451,	492,	532,	573,	614,	655,	696,	737,	778,	819,	860,	901,	942,	983,	1024,	1065,	1106,	1147,	1188,	1229,	1270,	1311,	1352,	1393,	1434,	1475,	1516,	1556,	1597,	1638,	1679,	1720,	1761,	1802,	1843,	1884,	1925,	1966,	2007,	2048,	2089,	2130,	2171,	2212,	2253,	2294,	2335,	2376,	2417,	2458,	2499,	2540,	2580,	2621,	2662,	2703,	2744,	2785,	2826,	2867,	2908,	2949,	2990,	3031,	3072,	3113,	3154,	3195,	3236,	3277,	3318,	3359,	3400,	3441,	3482,	3523,	3564,	3604,	3645,	3686,	3727,	3768,	3809,	3850,	3891,	3932,	3973,	4014,	4055, 4014,	3973,	3932,	3891,	3850,	3809,	3768,	3727,	3686,	3645,	3604,	3564,	3523,	3482,	3441,	3400,	3359,	3318,	3277,	3236,	3195,	3154,	3113,	3072,	3031,	2990,	2949,	2908,	2867,	2826,	2785,	2744,	2703,	2662,	2621,	2580,	2540,	2499,	2458,	2417,	2376,	2335,	2294,	2253,	2212,	2171,	2130,	2089,	2048,	2007,	1966,	1925,	1884,	1843,	1802,	1761,	1720,	1679,	1638,	1597,	1556,	1516,	1475,	1434,	1393,	1352,	1311,	1270,	1229,	1188,	1147,	1106,	1065,	1024,	983,	942,	901,	860,	819,	778,	737,	696,	655,	614,	573,	532,	492,	451,	410,	369,	328,	287,246,205,164,123,82,	41};

typedef struct
{

	void (*fptrSignal)(signal_form_t);
	signal_form_t next[2];

}State_signal;


const State_signal FSM_Signal[4]={
		    {type_signal, {OFF,SQUARE}},
            {  type_signal, {SQUARE,SIN}},  /* BLUE-SQUARE */
			{type_signal, {SIN,TRIANGLE}}, /* RED-SIN*/
			{type_signal, {TRIANGLE,SQUARE}}  /* VERDE-TRIANGLE */
		};


void Signal(void)
{ /* if g_signal_enable are not on true the state will be OFF*/
	sw3_interrupt = GPIO_get_sw3_intr();
	/*g_signal_enabled = keyboard_g_enabled();
	if (!g_signal_enabled)
	{
		current_state1 = OFF;
	}*/
	/* evaluate if there is a sw3 interruption to change the state to next*/
	if ((sw3_interrupt))
	{
		current_state1 = FSM_Signal[current_state1].next[NEXT_STATE];

		GPIO_clear_sw3_intr();

	}
	/*function to give a the signal that is in current_state1*/
	type_signal(current_state1);


	return;
}


/*to choose the signal depending of the type*/
void type_signal(signal_form_t type){
	switch (type){
	case SQUARE:
		SIGNAL_square();
		break;
	case SIN:
		SIGNAL_sin();
		break;
	case TRIANGLE:
		SIGNAL_triangle();
		break;
	default:
		SIGNAL_off();
		break;
}
	}

void Signal_enable(void){
	DAC_enable();
	PIT_clock_gating();
	PIT_enable();
	NVIC_enableInterruptAndPriotity(PIT_CH1_IRQ, PRIORITY_11);
	//EnableInterrupts;
}



void SIGNAL_sin(void){
	//RGB_on(RGB_RED);
int tam=100;
uint8 High, Low;
g_pit_intr_status_1 =PIT1_get_interrupt_glag_status();
/*if  g_signal_state reach the last position of the array it return to the initial position of the array*/
g_signal_state = (tam > g_signal_state)? g_signal_state : 0;
/*if the flag of the pit are true(PIT has finished) the g_signal_state goes to the next position of the array*/
g_signal_state=(g_pit_intr_status_1)? g_signal_state+1 : g_signal_state;
/*clear the flag of pit status*/
if(g_pit_intr_status_1){PIT1_clear_interrupt_flag();}
/*assign the values to LOW and HIGH */
	Low=(uint8)(sen[g_signal_state] & Low_MASK);
	High=((uint8) ((sen[g_signal_state]) >> 8) & High_MASK);
DAC_Set(High, Low);
/*turn on the PIT*/
PIT_delay(PIT_1,SYSTEM_CLOCK,DELAY3);
}


void SIGNAL_square(void){
	//RGB_on(RGB_BLUE);
	uint32_t tam=2;
	uint8 High, Low;
	g_pit_intr_status_1 =PIT1_get_interrupt_glag_status();
	g_signal_state = (tam > g_signal_state)? g_signal_state : 0;
	g_signal_state=(g_pit_intr_status_1)? g_signal_state+1 : g_signal_state;
	if(g_pit_intr_status_1)
	{
		PIT1_clear_interrupt_flag();
	}
	Low=(uint8)(square[g_signal_state] & Low_MASK);
	High=((uint8) ((square[g_signal_state]) >> 8) & High_MASK);
	DAC_Set(High, Low);
	PIT_delay(PIT_1,SYSTEM_CLOCK,DELAY1);
}

void SIGNAL_triangle(void){
	//RGB_on(RGB_GREEN);
	uint32_t tam=200;
	uint8 High, Low;
	g_pit_intr_status_1 =PIT1_get_interrupt_glag_status();
	g_signal_state = (tam > g_signal_state)? g_signal_state : 0;
	g_signal_state=(g_pit_intr_status_1)? g_signal_state+1 : g_signal_state;
	if(g_pit_intr_status_1)
	{
		PIT1_clear_interrupt_flag();
	}
	Low=(uint8)(saw[g_signal_state] & Low_MASK);
	High=((uint8) ((saw[g_signal_state]) >> 8) & High_MASK);
	DAC_Set(High, Low);
	PIT_delay(PIT_1,SYSTEM_CLOCK,DELAY2);
}

/*turn off the signal*/
void SIGNAL_off(void)
{
	//RGB_off();
	uint8 High, Low;
	Low=(0 & Low_MASK);
	High=(0 & High_MASK);
	DAC_Set(High, Low);
}








