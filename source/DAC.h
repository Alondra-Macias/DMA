/*
 * DAC.h
 *
 *  Created on: 22/09/2018
 *      Author: Alondra Macias, Margarita Jauregui
 */

#ifndef DAC_H_
#define DAC_H_
/*
 * DAC.c
 *
 *  Created on: 20/09/2018
 *      Author: AlondraItzel
 */
#include <stdio.h>
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
//0xC0
#define MASK_ENABLE_ON 1
#define MASK_REF_2 1
#define MASK_Software_Off 0
#define MASK_Soft_trigger_Off 0
#define MASK_flags 0x0

//1 enable on
//1 REF_2 selected
//1 software trigger is selected.

/*!
 	\brief This function configures the Dac clock and Dac enable
 	\param[in] void
 	\return void
 */
void DAC_enable();
/*!
 	\brief This function set the value for the DAC
 	\param[in] DAC Data Low Register (DACx_DATnL) 8 bits, DAC Data High Register (DACx_DATnH) 4 bits (cuantificacion 4096)
 	\return void
 */
void DAC_Set(uint8 dataH, uint8 dataL);



#endif /* DAC_H_ */
