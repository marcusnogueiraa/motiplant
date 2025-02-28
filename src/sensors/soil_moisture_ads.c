#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "sensors/soil_moisture_ads.h"

int soil_moisture_ads_init(SoilMoistureADS *sensor, uint8_t channel, int wet_value, int dry_value) {
    if ((sensor->i2c_fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Erro ao abrir o barramento I2C");
        return -1;
    }

    if (ioctl(sensor->i2c_fd, I2C_SLAVE, ADS1115_ADDRESS) < 0) {
        perror("Erro ao configurar comunicação com ADS1115");
        close(sensor->i2c_fd);
        return -1;
    }

    sensor->channel = channel;
    sensor->wet_value = wet_value;
    sensor->dry_value = dry_value;
    return 0;
}

int soil_moisture_ads_read_raw(SoilMoistureADS *sensor) {
    uint8_t config[3] = { 0x01, (0xC0 | (sensor->channel << 4)), 0x83 };
    if (write(sensor->i2c_fd, config, 3) != 3) {
        perror("Erro ao escrever no ADS1115");
        return -1;
    }
    
    usleep(8000);
    uint8_t reg[1] = {0x00};
    if (write(sensor->i2c_fd, reg, 1) != 1) {
        perror("Erro ao solicitar leitura do ADS1115");
        return -1;
    }

    uint8_t data[2];
    if (read(sensor->i2c_fd, data, 2) != 2) {
        perror("Erro ao ler dados do ADS1115");
        return -1;
    }

    int raw_value = ((data[0] << 8) | data[1]);
    if (raw_value > 32767) raw_value -= 65536;

    return raw_value;
}

int soil_moisture_ads_read_percentage(SoilMoistureADS *sensor) {
    int raw = soil_moisture_ads_read_raw(sensor);
    if (raw == -1) return -1;

    int percentage = 100 - ((raw - sensor->wet_value) * 100) / (sensor->dry_value - sensor->wet_value);
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    return percentage;
}

void soil_moisture_ads_close(SoilMoistureADS *sensor) {
    close(sensor->i2c_fd);
}

