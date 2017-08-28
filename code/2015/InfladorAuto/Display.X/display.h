#include<stdint.h>
#define DISPLAY_RELOJ 0
#define DISPLAY_PRESION 1
uint16_t armar4Digitos(uint16_t mil, uint16_t cen);
void sipo_send(uint16_t sendNro, uint16_t digitos, uint16_t display);
uint16_t waitSetpoint(uint16_t pedido);