/*Includes globales*/
#include <xc.h>
#include <pic18.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void ConfigUSART(void){
	TRISCbits.RC6 = 0; //TX es salida
	TRISCbits.RC7 = 1; //RX es entrada
	OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH,16);
    BAUDCONbits.BRG16 = 1;
}/* 9600 sobre 8Mh: USART_BRGH_HIGH,51 -> 51: 9615,38baudios
  * 115000 sobre 8Mh: USART_BRGH_HIGH,3.34 ->  3: 125000baudios
  * 34800 sobre 8Mh: USART_BRGH_HIGH,13.36 ->  13: 35714baudios
  * 57600 sobre 8Mh: USART_BRGH_HIGH,8 
  * 115000 sobre 8Mh: USART_BRGH_LOW,0.08  ->  0: 125000baudios
  */


void sendStr( const uint8_t *dataPtr)       // Transmitir un byte
{
  while( *dataPtr ){  
    while(BusyUSART());
    TXREG = *dataPtr;
    dataPtr++;
  }
}

//// quick and dirty ftoa for legacy code
//uint8_t * ftostr(float f)
//{
//	static uint8_t		buf[17];
//	uint8_t *			cp = buf;
//    uint32_t	l, rem;
//
//	if(f < 0) {
//		*cp++ = '-';
//		f = -f;
//	}
//	l = ( uint32_t)f;
//	f -= (float)l;
//	rem = ( uint32_t)(f * 1e6);
//	sprintf(cp, "%lu.%2.2lu", l, rem);
//	return buf;
//}
extern uint8_t retornoParametros;

    static volatile uint32_t lastTIK = 0;
void enviarEstadoConsola(uint32_t tik, uint32_t error, uint32_t dutyPWM, uint32_t ctrl,uint32_t Kp, uint32_t Ki, uint32_t Kd, uint32_t Kp2, uint32_t Ki2, uint32_t Kd2,uint8_t controlOn, uint32_t dirStep, uint32_t potenciaStep, uint32_t dutyPWMmin,uint32_t dutyPWMlimit, uint32_t dutyPWMlimitTimeout){
    uint8_t buffer[10];
    if (tik == lastTIK){return;}
    lastTIK = tik;
    sendStr("{");

	sendStr("\"t\":");    
	utoa(buffer, tik,10);
	sendStr(buffer);

    sendStr(",");
	sendStr("\"lt\":");    
	utoa(buffer, lastTIK,10);
	sendStr(buffer);

    sendStr(",");

	sendStr("\"e\":");    
	ltoa(buffer, error, 10);
	sendStr(buffer);

    sendStr(",");

	sendStr("\"c\":");   
	ltoa(buffer, ctrl, 10);
	sendStr(buffer);
/*
    sendStr(",");
    
	sendStr("\"p\":");
	ltoa(buffer, dutyPWM,10);
	sendStr(buffer);*/
    
    if (retornoParametros == 56){
       
        sendStr(",");
        
        sendStr("\"r\":");   
        itoa(buffer, retornoParametros, 10);
        sendStr(buffer);
        sendStr(",");
        
        retornoParametros=0;
         
        
        sendStr("\"kp\":");   
        ltoa(buffer, Kp, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"ki\":");   
        ltoa(buffer, Ki, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"kd\":");   
        ltoa(buffer, Kd, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"kp2\":");   
        ltoa(buffer, Kp2, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"ki2\":");   
        ltoa(buffer, Ki2, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"kd2\":");   
        ltoa(buffer, Kd2, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"cOn\":");   
        itoa(buffer, controlOn, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"ds\":");   
        ltoa(buffer, dirStep, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"ps\":");   
        ltoa(buffer, potenciaStep, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"dm\":");   
        ltoa(buffer, dutyPWMmin, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"dl\":");   
        ltoa(buffer, dutyPWMlimit, 10);
        sendStr(buffer);

        sendStr(",");

        sendStr("\"dt\":");   
        ltoa(buffer, dutyPWMlimitTimeout, 10);
        sendStr(buffer);
    
    }
    
	sendStr("}");
    sendStr("\n\r");      // Fin de linea
}


