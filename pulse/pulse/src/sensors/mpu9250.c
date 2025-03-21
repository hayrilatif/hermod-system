#include "../../include/sensors/mpu9250.h"
#include "../../include/default_pins.h"
#include "../../include/shared_data.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"

#define DMA_CHANNEL 10
#define MPU9250_ADDRESS 0x68
#define MPU9250_READ_LENGTH 14
#define I2C_PORT i2c0

uint8_t buffer[MPU9250_READ_LENGTH];
mpu9250_sensor_data_t sensor_data;

void setup_dma(){
dma_channel_config c = dma_channel_config_get_default_config(DMA_CHANNEL);
    
channel_config_set_transfer_data_size(&c,DMA_SIZE_8);
channel_config_set_read_increment(&c,false);
channel_config_set_write_increment(&c,true);
channel_config_set_dreq(&c,DREQ_I2C0_RX);
    
dma_channel_configure(DMA_CHANNEL,&c,buffer,&i2c0_hw->data_cmd,MPU9250_READ_LENGTH,false);   
}

//ortalama 40ms okuma suresi (biraz abartarak hesapladim)
bool read_mpu9250(mpu9250_sensor_data_t* data)
{
   
    uint8_t reg = MPU9250_ACCEL_XOUT_H;
    
    // okuma baslangici adresini ver &reg
    int ret = i2c_write_blocking(I2C_PORT, MPU9250_ADDRESS, &reg, 1, true);
    
    if(ret < 0) {
        return false;
    }
    dma_channel_transfer_to_buffer_now(DMA_CHANNEL,buffer,MPU9250_READ_LENGTH);
    // oku
   process_mpu_data(data);
    return true;
    }
    
}
void process_mpu_data (mpu9250_sensor_data_t* data) {
    data->accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);
    data->temp    = (int16_t)((buffer[6] << 8) | buffer[7]);
    data->gyro_x  = (int16_t)((buffer[8] << 8) | buffer[9]);
    data->gyro_y  = (int16_t)((buffer[10] << 8) | buffer[11]);
    data->gyro_z  = (int16_t)((buffer[12] << 8) | buffer[13]);
    printf("Accel X: %d, Accel Y: %d, Accel Z: %d\n", accel_x, accel_y, accel_z); 
}

bool mpu9250_htimer_callback(struct repeating_timer *t)
{
    bool is_read_successfull = read_mpu9250(&sensor_data); //timestamp harici sensor_data doldur

    if (is_read_successfull)
    {
        //basarili
        sensor_data.timestamp = time_us_32();
        buffer_shift_and_update(&mpu9250_mutex, (uint8_t*) mpu9250_sensor_buffer, &sensor_data, sizeof(mpu9250_sensor_data_t));
    }
    else
    {
        //basarisiz
    }

    return true; //timer tekrar tekrar calisir (true)
}
// main program 
int main() {
stdio_init_all();
i2c_init(I2C_PORT,400*1000); // 400 kHz I2C hızı
gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN,GPIO_FUNC_I2C);
gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

setup_dma();
while(1){
    read_mpu9250();
    sleep_ms(40); // 40 ms bekleme süresi
    process_mpu_data(&sensor_data);
}
}




    
}
