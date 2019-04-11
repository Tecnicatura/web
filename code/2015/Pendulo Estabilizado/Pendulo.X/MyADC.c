
/*Includes globales*/
#include <xc.h>
#include <pic18.h>
#include "adc.h"
#include <stdlib.h>
#include <stdint.h>

void ConfigADC(void){
	OpenADC(ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_2_TAD, ADC_INT_ON & ADC_REF_VDD_VSS, ADC_7ANA);
}

void comenzarADC(void){
	SetChanADC (ADC_CH5);	// Selecciono canal a convertir.-
	ConvertADC();			// Comienza conversion.-
}

uint32_t getADC(){
	return (uint32_t)ReadADC();
}