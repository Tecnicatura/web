
#define _XTAL_FREQ 8000000 // Linea necesaria para los c�lculos de la funci�n
// de delay. CUIDADO. no es para la configuraci�n
// de la frecuencia de trabajo del cpu del pic.
// eso se determina en otro lado. :)

// CONFIG1L
#pragma config PLLDIV = 2       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS     // Oscillator Selection bits (XT oscillator (XT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)



/*Includes globales*/
#include <xc.h>
#include <pic18.h>
//#include <plib/usart.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MyControl.h"
#include "MyUSART.h"
#include "MyADC.h"
#include "MyPWM.h"
#include<stdint.h>

#define SETBIT(puerto, nbit) (puerto|=(1<<nbit))
#define CLRBIT(puerto, nbit) (puerto&=~(1<<nbit))
///////////////////////////////////////////////////////////////////////////////
//                     Programa Principal                                    //
///////////////////////////////////////////////////////////////////////////////

#define MAX_BUFFER 10 
#define TOUT_INICIAL 1
#define TIEMPO_RESET_INTEGRAL 200
uint8_t MessageBuffer[MAX_BUFFER]; // Buffer de entrada de uint8_ts por el RX del USART
uint8_t globalIndexRx = 0;
uint8_t banderaParser = 0;
//uint8_t subScaler = 0;
uint8_t verbose = 0;
uint32_t Kp = 1000, Ki = 0, Kd = 0;
uint32_t Kp2 = 1, Ki2 = 0, Kd2 = 0;
int32_t iError1;
int32_t iError2;
int32_t error;
uint32_t setPoint = 512;
uint32_t posicion = 512;
uint32_t ctrl = 0;
uint32_t dutyPWM;
uint32_t tik, tik0, tik0i, dt;
int16_t contadorCiclosControlOff;
uint8_t controlOn = 1;
uint8_t controlAntesDelStep = 1;
uint32_t dirStep;
uint32_t potenciaStep;
uint32_t dutyPWMmin = 0;
uint32_t dutyPWMlimit = 768; // Valor de dutyPWM sobre el cual comienza a contar el Timeout
uint32_t dutyPWMlimitTimeout = TOUT_INICIAL; // Cantidad de ciclos que dutyPWM debe superar el limite antes de apagar el control
uint32_t dutyPWMlimitCounter = TOUT_INICIAL; // Variable auxiliar para contar el Timeout sin perder la configuracion
volatile int8_t retornoParametros = 0; // Cantidad de veces que se debe enviar el estado completo de configuracion por el puerto
uint8_t dentroDeStep;
volatile uint8_t doPID;



void reset_integral(void){
    
            iError1 = 0;
            iError2 = 0;
            tik0i= tik;
}
void setActuador(uint32_t dutyPWM, uint8_t dirGiro);
void accionDeControl(uint8_t dirGiro);
void parserRx(void) {
    uint8_t comando;
    uint8_t j = 1;
#define MAX_ARG 5
    uint8_t argumento[MAX_ARG];
    banderaParser = 0;
    comando = MessageBuffer[0];
    do {
        argumento[j - 1] = MessageBuffer[j];
        j++;
        if (j > MAX_ARG) break;
    } while (MessageBuffer[j] != '\0');
    argumento[j-1] = '\0';
    for (; globalIndexRx > 0; globalIndexRx--)
        MessageBuffer[globalIndexRx] = 0x00; //clear the array

    switch (comando) {
        case 'p':
            Kp = (uint32_t) atol(argumento);
            break;
        case 'i':
            Ki = (uint32_t) atol(argumento);
            break;
        case 'd':
            Kd = (uint32_t) atol(argumento);
            break;
        case 'q':
            Kp2 = (uint32_t) atol(argumento);
            break;
        case 'w':
            Ki2 = (uint32_t) atol(argumento);
            break;
        case 'e':
            Kd2 = (uint32_t) atol(argumento);
            break;
        case 'z':
            setPoint = posicion;
            break;
        case 'c':
            TMR1IE = 0;
            controlOn = (uint8_t) atoi(argumento);
            controlAntesDelStep=controlOn;
            setActuador(0, 0);
            TMR1IE = 1;
            break;
        case 'v':
            verbose = (uint8_t) atoi(argumento);
            break;
        case 'n':
            potenciaStep = (uint32_t) atol(argumento);
            break;
        case 'm':
            dirStep = (uint32_t) atol(argumento);
            break;
        case 's':
            TMR1IE = 0;
            contadorCiclosControlOff = (uint32_t) atol(argumento);
            controlAntesDelStep = controlOn;
            controlOn = 0;
            dentroDeStep = 1;
            reset_integral();
            setActuador(potenciaStep, dirStep);
            TMR1IE = 1;
            break;
        case '1':
            dutyPWMlimit = (uint32_t) atol(argumento);
            break;
        case '2':
            dutyPWMlimitTimeout = (uint32_t) atol(argumento);
            break;
        case '3':
            dutyPWMmin = (uint32_t) atol(argumento);
            break;
        case 'r':
            retornoParametros = (uint8_t) atoi(argumento);
            break;
        case 'a':
            TMR1IE = 0;
            controlOn = 0;
            int32_t potencia = (int32_t) atol(argumento);
            uint32_t dirGiro = 0;
            if (potencia < 0){
                dirGiro = 1;
            } else {
                dirGiro = 2;
            }
            setActuador((uint32_t) labs(potencia), dirGiro);
            TMR1IE = 1;
            break;

    }

}

void borrarBufferRx() {

    for (int i = 0; i < MAX_BUFFER; i++)
        MessageBuffer[i] = 0x00; // Borra el buffer
    globalIndexRx = 0; //for sanity
}

void main(void) {
    TRISA = 0xFF; // Puerto A como entradas
    //LATA=0b00000000;      //todas las salidas abajo
    TRISB = 0xFF; // Puerto B como entradas
    //LATB=0b00000000;      //todas las salidas abajo	 
    TRISC = 0xFF; // Puerto C como entradas
    //LATC=0b00000000;      //todas las salidas abajo
    TRISD = 0xFF; // Puerto D como entradas
    //LATD=0b00000000;      //todas las salidas abajo


    CLRBIT(TRISD, 0); // RD0 como salida: Direccion de giro
    CLRBIT(TRISD, 1); // RD1 como salida: Direccion de giro
    CLRBIT(TRISC, 2); // RC2 como salida: PWM
    CLRBIT(TRISC, 6); // RC6 como salida: USART Tx


    OpenTimer1(T1_PS_1_8 & T1_16BIT_RW & TIMER_INT_ON);
    // Inicio el Timer1
    T1CONbits.TMR1ON = 1; //Configure Timer1 interrupt
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1OSCEN = 0;
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 0;
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCON2bits.T0IP = 0;
    RCONbits.IPEN = 0x01;


    IPR1bits.TMR1IP = 0x00; //0x01;            // TMR1 high priority ,TMR1 Overflow Interrupt Priority bit
    PIR1bits.TMR1IF = 0;

    // Inicio el USART
    ConfigUSART();
    // Inicio el PWM
    ConfigPWM();
    // Inicio el ADC
    ConfigADC();
    // Comienzo conversion ADC
    comenzarADC();

    // Configuracion de Interrupciones
    PIE1bits.ADIE = 0; // DISABLE ADC interrupts
    PIE1bits.RCIE = 1; // ENABLE Rx interrupts
    RCONbits.IPEN = 1; // ENABLE interrupt priority
    INTCONbits.PEIE = 1; // ENable peripheral interrupts.
    INTCONbits.GIE = 1; // ENABLE global interrupts

    verbose = 1;
    while (1) { // Bucle Principal

        if (doPID != 0) {
            doPID = 0;
            uint8_t dirGiro;
            ctrl = controlador(error,dt,
                    Kp, Ki, Kd,
                    &dutyPWM, &dirGiro); // Almaceno en control el resultado del algoritmo de control
            accionDeControl(dirGiro);
        }

        if (verbose == 1) {
            enviarEstadoConsola(tik, error, dutyPWM, ctrl,
                    Kp, Ki, Kd, Kp2, Ki2, Kd2,
                    controlAntesDelStep, dirStep, potenciaStep,
                    dutyPWMmin, dutyPWMlimit, dutyPWMlimitTimeout);
        } // Envio los parametros por USART
        if (banderaParser == 1) {
            banderaParser = 0;
            parserRx();
            borrarBufferRx();
        }
    }
}

void setActuador(uint32_t dutyPWM, uint8_t dirGiro) {
    dutyPWM += dutyPWMmin;
    if (dutyPWM < 1024) {
        setDUTY((unsigned int) dutyPWM);
    } else {
        setDUTY(1023);
    }

    switch (dirGiro) {
        case 0:
            PORTDbits.RD0 = 0; // No -
            PORTDbits.RD1 = 0; // gira
            break;
        case 1:
            PORTDbits.RD0 = 1; // Gira para
            PORTDbits.RD1 = 0; // un lado
            break;
        case 2:
            PORTDbits.RD0 = 0; // Gira para
            PORTDbits.RD1 = 1; // otro lado
            break;
        default:
            PORTDbits.RD0 = 0; // No -
            PORTDbits.RD1 = 0; // gira
            break;
    }
}

void accionDeControl(uint8_t dirGiro) {
    if (dutyPWM > dutyPWMlimit) { // Seguridad: si el PWM mayor a limit durante timeout ciclos, apaga el control

        dutyPWMlimitCounter--;
        if (dutyPWMlimitCounter == 0) {
            setActuador(0, 0); // Apago el motor
            controlOn = 0; // Desactivo el control
        }
    } else {
        dutyPWMlimitCounter = dutyPWMlimitTimeout;
    }

    if (dentroDeStep != 0) {
        if (contadorCiclosControlOff <= 0) {
            setActuador(0, 0); // Apago el motor
            controlOn = controlAntesDelStep; // reactivo el control
            dentroDeStep = 0;
        } else {
            contadorCiclosControlOff--;
        }
    }
    if (controlOn == 1) {
        setActuador(dutyPWM, dirGiro);
    }
}

void interrupt low_priority LowIsr(void) //Interrupciones de baja prioridad (no logro hacer funcionar las de alta)
{
    if (TMR1IE && PIR1bits.TMR1IF) // Si interrumpe por desborde del Timer1
    {
        PIR1bits.TMR1IF = 0; // Bajo la bandera
        //if (subScaler == 10){
        //subScaler = 0;
        tik++;

        if (PIR1bits.ADIF != 0) {

            PIR1bits.ADIF = 0; // Bajo la bandera 
            posicion = getADC();
            error = (setPoint - posicion); // Calculo el error
            comenzarADC(); // Comienzo una nueva conversion inmediatamente
            dt = tik - tik0;
            
            tik0 = tik;
            if(tik-tik0i>TIEMPO_RESET_INTEGRAL){
                reset_integral();}
            doPID = 1;
        }
        //}else{subScaler ++;}
    }

    //    if(PIR1bits.ADIF == 1){         // Si interrumpe por conversion ADC
    //        PIE1bits.ADIE=lastError2;0;  
    //        
    //    }

    if (PIR1bits.RCIF == 1) // Si interrumpe por entrada de dato en USART
    {
        PIR1bits.RCIF = 0;
        uint8_t caracterRx;
        caracterRx = ReadUSART();
        if (caracterRx == 0x0D) { // Si apretas enter
            banderaParser = 1;
            //parserRx();
        } else if (globalIndexRx < MAX_BUFFER) {
            MessageBuffer[globalIndexRx] = caracterRx; //Lee el caracter de Rx
            globalIndexRx++;
            MessageBuffer[globalIndexRx] = '\0'; //Mantengo un 0 al final del str.
        } else {
            borrarBufferRx();
        }
    }

    if (PIR1bits.SPPIF == 1) {
        PIR1bits.SPPIF = 0;
    }
    if (PIR1bits.TXIF == 1) {
        PIR1bits.TXIF = 0;
    }
    if (PIR1bits.SSPIF == 1) {
        PIR1bits.SSPIF = 0;
    }
    if (PIR1bits.CCP1IF == 1) {
        PIR1bits.CCP1IF = 0;
    }
    if (PIR1bits.TMR2IF == 1) {
        PIR1bits.TMR2IF = 0;
    }
}
