#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "sensors/dht11.h"
#include "sensors/mpu9250.h"
#include "sensors/optics.h"
#include "pico/mutex.h"

void communication_configure();
void safe_uart_transfer(mutex_t *mutex, const void *array, size_t element_size);
bool general_comm_transfer_htimer_callback(struct repeating_timer *t);

#endif