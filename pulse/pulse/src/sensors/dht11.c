#include "../../include/sensors/dht11.h"
#include "../../include/default_pins.h"
#include "../../include/shared_data.h"

//ortalama 40ms okuma suresi (biraz abartarak hesapladim) timestamp doldurmaz digerlerini doldurur sensor data icin
bool read_dht11(dht11_sensor_data_t* sensor_data)
{
    uint8_t data[5] = {0};

    // dht start sinyali
    // sensor okuma istegi en az 18ms low
    gpio_set_dir(DHT11_PIN, GPIO_OUT);
    gpio_put(DHT11_PIN, 0); //low
    sleep_ms(20);
    gpio_put(DHT11_PIN, 1);
    sleep_us(40);      // 40 us high
    // veri ookuma icin giris.
    gpio_set_dir(DHT11_PIN, GPIO_IN);

    // yanit bekleme
    // 80us low 80us high verir.
    uint32_t timeout = time_us_32() + 100;
    // low sinyal bekle
    while (gpio_get(DHT11_PIN)) {
        if (time_us_32() > timeout)
            return false;  //zaan asimi
    }
    timeout = time_us_32() + 100;
    // high sinyal bekle
    while (!gpio_get(DHT11_PIN)) {
        if (time_us_32() > timeout)
            return false;
    }
    timeout = time_us_32() + 100;
    // lowa gececek
    while (gpio_get(DHT11_PIN)) {
        if (time_us_32() > timeout)
            return false;
    }

    // 40 bit veri okuma
    // her bit baslangici 50us low sinyal high süresine gore 0 veya 1
    for (int i = 0; i < 40; i++) {
        // low sinyalin bitmeini bekle
        timeout = time_us_32() + 100;
        while (!gpio_get(DHT11_PIN)) {
            if (time_us_32() > timeout)
                return false;
        }
        // high basladiginde olcume basla
        uint32_t start_time = time_us_32();
        timeout = time_us_32() + 100;
        while (gpio_get(DHT11_PIN)) {
            if (time_us_32() > timeout)
                break;
        }
        uint32_t pulse_length = time_us_32() - start_time;
        // 30us ise 0 | 70 ise 1 esigi 45 sectim 
        int bit_value = (pulse_length > 45) ? 1 : 0;
        data[i / 8] = (data[i / 8] << 1) | bit_value;
    }

    // checksum
    // ilk 4 toplami 5.e esit olacak
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4])
        return false;


    sensor_data->humidity = (float)data[0];
    sensor_data->temperature = (float)data[2];

    return true;
}

bool dht11_htimer_callback(struct repeating_timer *t)
{
    dht11_sensor_data_t sensor_data;

    bool is_read_successfull = read_dht11(&sensor_data);

    if (is_read_successfull)
    {
        sensor_data.timestamp = time_us_32();
        buffer_shift_and_update(&dht11_mutex, (uint8_t*) dht11_sensor_buffer, &sensor_data, sizeof(dht11_sensor_data_t));
        //basarili
    }
    else
    {
        //basarisiz
    }

    return true; //timer tekrar tekrar calisir (true)
}