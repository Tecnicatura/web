#include"display.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // Librería XC8
#include <stdint.h>
#define _XTAL_FREQ 4000000
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//#define testbit(r,b) ( r & (1<<b) )
//#define setbit(r,b) r |= (1<<(b))
//#define clrbit(r,b) r &= ~(1<<(b))

#define ST_ESPERA_COIN 0
#define ST_WAIT_SET_POINT 1
#define ST_CONTROLANDO 2
#define ST_TERMINADO 3
#define ST_SENSADO 4

#define TMR0OF  INTCONbits.TMR0IF
#define INFLA PORTCbits.RC1
#define DESINFLA PORTCbits.RC2
#define BUZZERFIN PORTCbits.RC4
#define COMPRESOR PORTCbits.RC3

int16_t diferenciaPresion;
uint16_t spnum;
uint16_t cuentaMS;
uint16_t tiempoDeAccion;
uint16_t estado;
int16_t accion;
uint16_t presionMedida;
uint16_t convertToUInt;
int16_t cargaPorSegundo =20; //presion por segundo



int16_t controlLoop(uint16_t spnum, uint16_t presionMedida0);

float Kp=0.792f;//max 2000

/*

 */
int16_t controlLoop(uint16_t spnum, uint16_t presionMedida0) {
    int16_t error;
    error = (spnum - presionMedida0);
//return (int16_t) ((float)(error)*Kp);
    return error;
}
void interrupt ISR() {
    if (TMR0OF != 0) { //este timer ocurre cada 1ms
        TMR0OF = 0;

        if (estado == ST_CONTROLANDO) {
            cuentaMS++;
            tiempoDeAccion++;
        }
        TMR0 = 6;
    }
    if (ADIF != 0) { //ADIF es el flag del CAD
        ADIF = 0;
        presionMedida = (uint16_t)ADRESL + (uint16_t)((ADRESH) << 8);
        convertToUInt= (uint16_t)((float)presionMedida/(float)10.33f);
        ADON = 1;
    }
}


uint16_t actuador(int16_t accion) {
    if (accion > 0) {
        COMPRESOR = 1;
        INFLA = 1;
        DESINFLA = 1;
        return 0;
    } else if (accion < 0) {
        INFLA =0 ;
        COMPRESOR = 0;
        DESINFLA = 0;
        return 0;
    } else {
        INFLA = 0;
        COMPRESOR = 0;
        DESINFLA = 1;
        sipo_send((uint16_t)convertToUInt,
                                2/*digitos*/,
                                DISPLAY_PRESION/*nro display*/);
        BUZZERFIN =1;
       __delay_ms(500);
        BUZZERFIN =0;
        return 1;
     }
}



#define FICHA PORTCbits.RC5
#define ACEPTAR PORTCbits.RC0

void main(void) {
  
   
    TRISB = 0b10000001; // Configuro puerto B
    TRISC = 0b00000001; // configuro puerto C
    TMR0 = 6;
    INTCONbits.TMR0IE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.T0CS = 0;

    ADIE = 1; //hab interrupt CAD
    ADON= 1; //enable CAD
    PCFG0 = 0; //config entrada CAD
    PCFG1 = 0; //config entrada CAD
    PCFG2 = 0; //config entrada CAD
    PCFG3 = 0; //config entrada CAD
    ADCS0 = 1; //seleccion de frec muestreo
    ADCS1 = 1; //seleccion de frec muestreo
    ADCS2 = 0; //seleccion de frec muestreo
    ADFM = 1; //justificado a la derecha

    CHS0 = 0; //seleccion de canal de entrada
    CHS1 = 0; //seleccion de canal de entrada
    CHS2 = 0; //seleccion de canal de entrada
    
    uint16_t min;
    uint16_t seg;
    

     estado = ST_ESPERA_COIN;
    while(1){
        switch(estado){
            case ST_ESPERA_COIN:
                cuentaMS = 0;
                spnum = 0;
                COMPRESOR = 0;
                INFLA = 0;
                DESINFLA = 1;
                sipo_send(0/*todo en cero*/, 4/*envia hora en cero*/, DISPLAY_RELOJ);
                sipo_send(spnum/*todo en cero*/,2/*digitos*/,DISPLAY_PRESION/*nro display*/);
                if(FICHA != 0){
                    min = 03;
                    seg = 00;
                    estado=ST_WAIT_SET_POINT;
                 }
                break;

            case ST_WAIT_SET_POINT:
                spnum = waitSetpoint(spnum);
                if (ACEPTAR != 0 && spnum!=0) {
                    estado = ST_CONTROLANDO;
                }
                break;

            case ST_SENSADO:
                    COMPRESOR = 0;
                    __delay_ms (200);
                    GO_DONE = 1;
                    estado = ST_CONTROLANDO;
                    break;

            case ST_CONTROLANDO:
                    diferenciaPresion = controlLoop(spnum,convertToUInt);
                    if(actuador(diferenciaPresion)!=0){
                         estado = ST_WAIT_SET_POINT;
                    }
                    int16_t refresh;
                    if (cuentaMS >= 1000) {//cada 1 segundos
                        refresh ++;
                        cuentaMS = 0;
                        if (refresh >= 3){
                            refresh = 0;
                            estado = ST_SENSADO;
                        }
                        
                        sipo_send(armar4Digitos((uint16_t) min, (uint16_t) seg),
                                (uint16_t)4/*digitos*/,
                                (uint16_t)DISPLAY_RELOJ/*nro display*/);

                        if (seg > 0) {
                            seg--;
                        
                              sipo_send((uint16_t)convertToUInt,
                                2/*digitos*/,
                                DISPLAY_PRESION/*nro display*/);

                        } else {
                            if (min > 0) {
                                min--;
                                seg = 59;
                            } else {
                              actuador(0);
                              estado = ST_ESPERA_COIN;
                            }
                        }
                    }
                break;         
        }
      }
   }