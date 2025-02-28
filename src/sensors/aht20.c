#include "aht20.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

int aht20_init(AHT20 *sensor) {
    if ((sensor->i2c_fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Erro ao abrir o barramento I2C");
        return -1;
    }

    if (ioctl(sensor->i2c_fd, I2C_SLAVE, AHT20_ADDR) < 0) {
        perror("Erro ao conectar ao AHT20");
        close(sensor->i2c_fd);
        return -1;
    }

    return 0;
}

double aht20_read_temperature(AHT20 *sensor) {
    char cmd[3] = {0xAC, 0x33, 0x00};
    char data[6];

    if (write(sensor->i2c_fd, cmd, 3) != 3) {
        perror("Erro ao iniciar leitura do AHT20");
        return -1000.0;
    }
    usleep(80000); 

    if (read(sensor->i2c_fd, data, 6) != 6) {
        perror("Erro ao ler dados do AHT20");
        return -1000.0;
    }

    int raw_temp = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5];
    return ((raw_temp * 200.0) / 1048576.0) - 50.0;
}

void aht20_close(AHT20 *sensor) {
    if (sensor->i2c_fd >= 0) {
        close(sensor->i2c_fd);
    }
}
