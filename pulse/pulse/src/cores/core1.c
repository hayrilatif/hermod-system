//low freq az onemli islemler

#include "pico/stdlib.h"
#include "pico/time.h"

#include "../../include/cores/core1.h"

#include "../../include/sensors/dht11.h"
#include "../../include/sensors/mpu9250.h"

#include "../../include/communication.h"

void core1_entry()
{
    struct repeating_timer dht11_timer, mpu9250_timer;

    // 1000ms de bir optik verisi guncellenir (sicaklik, nem)
    if (!add_repeating_timer_ms(1000, dht11_htimer_callback, NULL, &dht11_timer)) {
        printf("Failed to add dht11 repeating timer\n");
        return core1_entry();
    }

    // 1000ms de bir optik verisi guncellenir (sicaklik, nem)
    if (!add_repeating_timer_us(1000, mpu9250_htimer_callback, NULL, &mpu9250_timer)) {
        return core1_entry();
    }

    //buraya +seyir bilgisari iletisim timer gelecek
    //her 100ms de bir tum sensor verilerinden en yeni bilgi gonderilecek.
    struct repeating_timer general_comm_transfer_timer;
    if (!add_repeating_timer_ms(100, general_comm_transfer_htimer_callback, NULL, &general_comm_transfer_timer)) {
        return core1_entry();
    }

    // interruptlar engellenmez
    while (true) {
        tight_loop_contents();
    }
}