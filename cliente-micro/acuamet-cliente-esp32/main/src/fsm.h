#ifndef FSM_H
#define FSM_H
#include "../main.h"

// estados
#define EST_INIT 0
#define EST_CONFIG 1
#define EST_WIFICONN 2
#define EST_MQTTCONN 3
#define EST_ONLINE 4
#define EST_OFFLINE 5
#define EST_PUBMQTT 6
#define EST_ERROR 7

#define delay_estados 100
#define delay_estado_mqttconn 1000
#define delay_estado_online 1000
#define delay_estado_offline 5000

extern int ESTADO_SIGUIENTE;
extern int ESTADO_ACTUAL;
extern int ESTADO_ANTERIOR;

int fun_init();
int fun_config();
int fun_wificonn();
int fun_mqttconn();
int fun_online();
int fun_offline();
int fun_pubmqtt();
int fun_error();

#endif