#ifndef MPU9250_H
#define MPU9250_H

#include "pico/stdlib.h"
#include "hardware/timer.h"

//pointer depola ileride
//mpu icin veri tutma structi
typedef struct {
    uint32_t timestamp;   // okuma zaman adimi  us
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t temp;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} mpu9250_sensor_data_t;

//mpu verisini okur ve sensor data structina yazar.
bool read_mpu9250(mpu9250_sensor_data_t* sensor_data);

//mpu okumasi icin donanimsal zamanlayici callback
bool mpu9250_htimer_callback(struct repeating_timer *t);


#endif