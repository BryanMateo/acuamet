#ifndef SENSORES_H
#define SENSORES_H
#include "../main.h"

#define promedio_nivel_cisterna 10
#define SENS_ERR_RANGE -1   // lectura del sensor fuera de rango
#define SENS_ERR_TIMEOUT -2 // tiempo de espera agotado

extern volatile uint64_t tiempo_inicio;
extern volatile uint64_t tiempo_fin;
extern volatile uint64_t duracion_pulso;

void set_pin_pcnt();
void set_pin_presion();
float read_pin_presion();
int nivel_cisterna_distance();



#endif