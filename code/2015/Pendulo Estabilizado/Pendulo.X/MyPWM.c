
/*Includes globales*/
#include <xc.h>
#include <pic18.h>
//#include <plib/pwm.h>
#include <stdlib.h>
//#include <plib/timers.h>

void ConfigPWM(void){
	SetDCPWM1 (0x00);
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_16 );
	OpenPWM1(0xFF);
}

void setDUTY(unsigned int duty){
	SetDCPWM1 (duty);
}