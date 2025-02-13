#ifndef DHT11_H
#define DHT11_H

#include "pico/stdlib.h"
#include "hardware/timer.h"


typedef struct {
    uint32_t timestamp;   // okuma zaman adimi  us
    float temperature;
    float humidity;
} dht11_sensor_data_t;


bool read_dht11(dht11_sensor_data_t* sensor_data);

bool dht11_htimer_callback(struct repeating_timer *t);


#endif