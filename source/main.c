

#include "MK64F12.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Bits.h"
#include "PIT.h"
#include <stdio.h>

#define NUM_STEPS (200u)
#define SYSTEM_CLOCK (21000000u)
#define DELAY (.02F)
#define NEXT (1u)
#define DMA_CH0 (0x01u)
#define DMA_SOURCE_GPIO (51u)
#define TRIANGULAR_SOURCE &saw[0]
#define SIN_SOURCE &sen[0]
#define SQUARE_SOURCE &square[0]

/*Enumeration to refer waveform*/
typedef enum
{
	TRIANGLE,
	SIN,
	SQUARE
}waveform_t;

/*Values for triangle signal*/
uint16_t saw[200]={0,	41,	82,	123,	164,	205,	246,	287,	328,	369,	410,	451,	492,	532,	573,	614,	655,	696,	737,	778,	819,	860,	901,	942,	983,	1024,	1065,	1106,	1147,	1188,	1229,	1270,	1311,	1352,	1393,	1434,	1475,	1516,	1556,	1597,	1638,	1679,	1720,	1761,	1802,	1843,	1884,	1925,	1966,	2007,	2048,	2089,	2130,	2171,	2212,	2253,	2294,	2335,	2376,	2417,	2458,	2499,	2540,	2580,	2621,	2662,	2703,	2744,	2785,	2826,	2867,	2908,	2949,	2990,	3031,	3072,	3113,	3154,	3195,	3236,	3277,	3318,	3359,	3400,	3441,	3482,	3523,	3564,	3604,	3645,	3686,	3727,	3768,	3809,	3850,	3891,	3932,	3973,	4014,	4055, 4014,	3973,	3932,	3891,	3850,	3809,	3768,	3727,	3686,	3645,	3604,	3564,	3523,	3482,	3441,	3400,	3359,	3318,	3277,	3236,	3195,	3154,	3113,	3072,	3031,	2990,	2949,	2908,	2867,	2826,	2785,	2744,	2703,	2662,	2621,	2580,	2540,	2499,	2458,	2417,	2376,	2335,	2294,	2253,	2212,	2171,	2130,	2089,	2048,	2007,	1966,	1925,	1884,	1843,	1802,	1761,	1720,	1679,	1638,	1597,	1556,	1516,	1475,	1434,	1393,	1352,	1311,	1270,	1229,	1188,	1147,	1106,	1065,	1024,	983,	942,	901,	860,	819,	778,	737,	696,	655,	614,	573,	532,	492,	451,	410,	369,	328,	287,246,205,164,123,82,	41};
/*Values for sin signal*/
uint16_t sen[100]={2048,2177,2305,2432,2557,2681,2802,2920,3035,3145,3252,3353,3450,3541,3626,3705,3777,3843,3901,3952,3996,4032,4060,4080,4092,4095,4092,4080,4060,4032,3996,3952,3901,3843,3777,3705,3626,3541,3450,3353,3252,3145,3035,2920,2802,2681,2557,2432,2305,2177,2048,1919,1791,1664,1539,1415,1294,1176,1061,951,844,743,646,555,470,391,319,253,195,144,100,64,36,16,4,0,4,16,36,64,100,144,195,253,319,391,470,555,646,743,844,951,1061,1176,1294,1415,1539,1664,1791,1919};
/*Values for square signal*/
uint16_t square[2]={4095,0};
/*FSM_state´s initial state*/
waveform_t current_state = SIN;


/*Enumeration to refer waveform*/
typedef struct
{
	const uint16_t* s_address;
	uint32_t slast;
	uint16_t n_steps;
     waveform_t next[2];
}State_t;


/*Waveforms FSM*/
const State_t FSM_states[3]=
  {
	{TRIANGULAR_SOURCE, -400, 200, {TRIANGLE, SIN}},
	{SIN_SOURCE, -200,100, {SIN, SQUARE}},
	{SQUARE_SOURCE, -4, 2,{SQUARE, TRIANGLE}}
 };


/*ISR for interruption on PORTC*/
void PORTC_IRQHandler()
{
	current_state = FSM_states[current_state].next[1];
	GPIO_clear_interrupt(GPIO_C);
	DMAMUX->CHCFG[0] &= ~(DMAMUX_CHCFG_ENBL_MASK);
	DMA_init();

}

/*ISR for DMA0*/
void DMA0_IRQHandler(void)
{
}

/*Enable DMA and DMAMUX clock gating*/
void DMA_clock_gating(void)
{
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
}


/*DMA configuration*/
void DMA_init(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
						DMAMUX_CHCFG_TRIG_MASK|
					   DMAMUX_CHCFG_SOURCE(58);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(FSM_states[current_state].s_address);/*defines source data address*/
	DMA0->TCD[0].SOFF = 2;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&DAC0->DAT[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 0;/*destination address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(FSM_states[current_state].n_steps);// NUM_STEPS;/*CITER = 1*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(FSM_states[current_state].n_steps);/*BITER = 1*/
	DMA0->TCD[0].NBYTES_MLNO = 2;/*byte number*/

	DMA0->TCD[0].ATTR = 0x101;/*16 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = FSM_states[current_state].slast;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/


}




int main(void)
{
	gpio_pin_control_register_t sw2 = GPIO_MUX1 | GPIO_PE | GPIO_PS | DMA_FALLING_EDGE; /* GPIO configured to trigger the DMA*/

	//GPIO_clock_gating(GPIO_C);
//	GPIO_pin_control_register(GPIO_C, bit_6, &sw2);
	SW_setup();
	DMA_clock_gating();
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;
	DAC_enable();
	PIT_clock_gating();
	PIT_enable();
	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY);
	DMA_init(); /* Configure the T*/
    for (;;) {

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
