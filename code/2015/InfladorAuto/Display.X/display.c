#include <xc.h>
#include <stdint.h>

#include "display.h"

#define _XTAL_FREQ 4000000
#define CONTROLANDO 1
#define ESPERA_COIN 0
#define STROBE0  PORTBbits.RB3
#define DATA0    PORTBbits.RB2
#define CLOCK0   PORTBbits.RB1

#define STROBE1  PORTBbits.RB6
#define DATA1    PORTBbits.RB4
#define CLOCK1   PORTBbits.RB5

#define SUBE PORTBbits.RB0
#define BAJA PORTBbits.RB7



/**
 * Ingresa unidad de mil y centena y arma numero completo
 * @param mil
 * @param cen
 * @return
 */
uint16_t armar4Digitos(uint16_t mil, uint16_t cen) {
    return mil * 100 + cen;

}

uint8_t obtieneUnidad(uint16_t sendNro) {
    uint8_t digito;
    digito =  (uint8_t) (sendNro % 10); // Extrae el resto de dividir el numero por diez
    return digito;
}

uint16_t divide10DescartaUnidad(uint16_t sendNro) {
    sendNro /= 10; // Lo divide por diez para "perder" el numero extraido
    return sendNro;
}

void envio8bits(uint8_t x, int16_t display) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (display == 0) {
            CLOCK0 = 0;
        } else {
            CLOCK1 = 0;
        }
        if ((x & 0x80) != 0) { //MSB primero (big endian puede ser al revés empezar en 0x01 LSB primero)
            if (display == 0) {
                DATA0 = 1;
            } else {
                DATA1 = 1;
            }
        } else {
            if (display == 0) {
                DATA0 = 0;
            } else {
                DATA1 = 0;
            }
        }
        x = x << 1; //MSB first (si fuera LSB first seria x=x>>1
        if (display == 0) {
            CLOCK0 = 1;
        } else {
            CLOCK1 = 1;
        }
    }
}



uint8_t transformaDecTo7Seg(uint8_t nro) {
    switch (nro) {
        case 0:
            return 0b11000000;
        case 1:
            return 0b11111001;
        case 2:
            return 0b10100100;
        case 3:
            return 0b10110000;
        case 4:
            return 0b10011001;
        case 5:
            return 0b10010010;
        case 6:
            return 0b10000010;
        case 7:
            return 0b11111000;
        case 8:
            return 0b10000000;
        case 9:
            return 0b10011000;
        default:
            return 0;
    }
}

void sipo_send(uint16_t sendNro, uint16_t digitos, uint16_t display) {
    uint8_t unidad;
    if (display == DISPLAY_RELOJ) {
        STROBE0 = 0; //bajo el strobe
    } else {
        STROBE1 = 0; //bajo el strobe
    }

    for (uint16_t i = 0; i < digitos; i++) {
        unidad = obtieneUnidad(sendNro);
        sendNro = divide10DescartaUnidad(sendNro);
        envio8bits(transformaDecTo7Seg(unidad), display);
    }

    if (display == DISPLAY_RELOJ) {
        STROBE0 = 1; //bajo el strobe
    } else {
        STROBE1 = 1; //bajo el strobe
    }
    __delay_ms(1); //tiempo para que salgan los datos al buffer

    if (display == DISPLAY_RELOJ) {
        STROBE0 = 0; //bajo el strobe
    } else {
        STROBE1 = 0; //bajo el strobe
    }
}

uint16_t waitSetpoint(uint16_t pedido) {
     uint16_t spnum=pedido;
    
        if (SUBE != 0) {
            __delay_ms(190);
                     {
                if (spnum < 13) {
                    spnum++;
                     sipo_send(spnum,
                        2/*digitos*/,
                        DISPLAY_PRESION/*nro display*/);
                    }
            }
        }else if (BAJA != 0) {
            __delay_ms(190);
                        {
                if (spnum > 0) {
                    spnum--;
                     sipo_send(spnum,
                    2/*digitos*/,
                    DISPLAY_PRESION/*nro display*/);
                    }
            }
        }
    
    return spnum;
}