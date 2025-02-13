//high freq islemler
//high freq sensor ve yonergeler icin timer atanir.

#include "pico/stdlib.h"
#include "pico/time.h"

#include "../../include/cores/core0.h"

#include "../../include/sensors/optics.h"


void core0_entry()
{
    struct repeating_timer optics_timer; //optik timer okuma icin degil snapshot icin kullanilir, okuma dma uzerinden yurutulutur

    // 500us de bir optik verisi guncellenir
    if (!add_repeating_timer_us(500, optics_htimer_callback, NULL, &optics_timer)) {
        return core0_entry();
    }

    //buraya +yonerge yurutme sistemi timer gelecek

    // interruptlar engellenmez
    while (true) {
        tight_loop_contents();
    }
}



