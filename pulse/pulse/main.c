#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "pico/multicore.h"


#include "include/cores/core0.h"
#include "include/cores/core1.h"

int main()
{
    stdio_init_all();

    //main_loop
    core0_entry();

    //start other core
    multicore_launch_core1(core1_entry);
}
