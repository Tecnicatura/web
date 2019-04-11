#include <stdint.h>

//int CalcPID(unsigned int kp, unsigned int ki, unsigned int kd, unsigned int timedelta, unsigned int SetPoint, unsigned int Input);
int32_t controlador(int32_t error,
        uint32_t dt,
        uint32_t Kp,
        uint32_t Ki,
        uint32_t Kd,
        uint32_t *dutyPWM,
        uint8_t *dirGiro) ; // Determina la accion de control segun el valor del error