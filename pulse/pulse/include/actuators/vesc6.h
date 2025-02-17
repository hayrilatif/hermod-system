#ifndef VESC6_H
#define VESC6_H
#include "pico/stdlib.h"
#include "stdio.h"
#include "../default_pins.h"
#include "hardware/pwm.h"

void vesc6_configure(); //connect pico <-> vesc 6
uint8_t calculate_checksum(uint8_t *data, int len);
void set_motor_rpm(int rpm);

#endif