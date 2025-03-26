#ifndef SENSORES_H
#define SENSORES_H
#include "../main.h"

#define constante_ppl_flujometro 450
#define constante_gpl_flujometro 1703

#define pcnt_unit_flujo_1 PCNT_UNIT_0
#define pcnt_unit_flujo_2 PCNT_UNIT_1
#define pcnt_unit_flujo_3 PCNT_UNIT_2
#define pcnt_unit_flujo_4 PCNT_UNIT_3

#define promedio_nivel_cisterna 10
#define promedio_presion 10
#define SENS_ERR_RANGE -1   // lectura del sensor fuera de rango
#define SENS_ERR_TIMEOUT -2 // tiempo de espera agotado

extern volatile uint64_t tiempo_inicio;
extern volatile uint64_t tiempo_fin;
extern volatile uint64_t duracion_pulso;

void set_pin_pcnt();      // establece la unidad contadora del flujometro
float calc_galon_flujo(); // lee el contador del flujometro y devuelve los galones contados

void set_pin_presion();   // configura el pin para el sensor de presion
float read_pin_presion(); // lee utilizando el adc el voltaje del sensor de presion y devuelve en PSI

int nivel_cisterna_distance();

#endif