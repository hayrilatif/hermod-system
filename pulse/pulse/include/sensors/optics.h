#ifndef OPTICS_H
#define OPTICS_H

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include <stdbool.h>


typedef struct {
    uint32_t timestamp;   // okuma zaman adimi  us
    float intensity0;
    float intensity1;
} optics_sensor_data_t;



#endif