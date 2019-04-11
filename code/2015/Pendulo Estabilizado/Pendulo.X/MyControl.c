#define MEDIO 512
#define MAYOR 768   //576
#define MENOR 128   //448


#include <xc.h>
#include <pic18.h>
#include <stdlib.h>
#include <math.h>
#include<stdint.h>


// float error;

int32_t calcPID(int32_t error,
        uint32_t dt,
        uint32_t Kp,
        uint32_t Ki,
        uint32_t Kd,
        int32_t *lastErrorPtr,
        int32_t *iErrorPtr) {
    
    int32_t output;
    int32_t dError;
    int32_t epsilon = 10000;
    int32_t omega = 200000;


    dError = (error - (*lastErrorPtr)); // Derivada del error
/*Ejemplo tiempo de integracion: 260ticks*/
    if ( /*(labs(error) < epsilon) ||*/ (labs(error) > omega) ) {
       *iErrorPtr =0;
    }else{
       *iErrorPtr = *iErrorPtr + (error * dt);
    }
        
        
    output = (Kp * error
            + Ki * (*iErrorPtr)
            + ((Kd * dError) / dt) ); // Salida

    *lastErrorPtr = error;

    return output;
}

extern uint32_t Kp2, Ki2, Kd2;
extern int32_t iError1;
extern int32_t iError2;

int32_t controlador(int32_t error,
        uint32_t dt,
        uint32_t Kp,
        uint32_t Ki,
        uint32_t Kd,
        uint32_t *dutyPWM,
        uint8_t *dirGiro) { // Determina la accion de control segun el valor del error
    
    int32_t accionControl;
    static int32_t lastError1;
    
    error *= 1000;  // agregamos 3 decimales al error
    
    accionControl = calcPID(error,
        dt,
            Kp, Ki, Kd,
            &lastError1,
            &iError1); // Devuelve la accion de control a realzar        

    static int32_t lastError2;

    accionControl = calcPID(accionControl,
            dt,
            Kp2, Ki2, Kd2,
            &lastError2,
            &iError2); // Devuelve la accion de con*/
    
    accionControl /= 1000000;  // Le quito los decimales agregados para los calculos

    if (accionControl > 0) {
        *dirGiro = 1;
    } else if (accionControl < 0) {
        *dirGiro = 2;
    } else {
        *dirGiro = 0;
    }
    
    *dutyPWM = (uint32_t) labs(accionControl);// Tomo el valor absoluto de resultado y lo asigno al duty
    return accionControl;
    //}
    //*dutyPWM=0;
    //return;
}

