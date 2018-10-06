/**
	\file
	\brief
		This is the source file for the DAC device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the DAC peripheral.
	\authors  Alondra Macias, Margarita Jauregui
	\date	30/09/2018
 */

#include "DAC.h"


void DAC_enable(void){
	 SIM->SCGC2 = SIM_SCGC2_DAC0_MASK;//clockDAC
	 DAC0->C1 = 0;
	 DAC0->C2 = 0;
	 DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

void DAC_Set(uint8 dataH, uint8 dataL){
	DAC0->DAT[0].DATH =(dataH); //offset de un 1h
	DAC0->DAT[0].DATL =(dataL);
	//DAC Data Low Register (DACx_DATnL) 8 bits
	//DAC Data High Register (DACx_DATnH) 4 bits
}

