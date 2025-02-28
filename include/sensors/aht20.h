#ifndef AHT20_H
#define AHT20_H

#include <stdint.h>

#define I2C_BUS "/dev/i2c-2"
#define AHT20_ADDR  0x38

typedef struct {
    int i2c_fd;
} AHT20;

int aht20_init(AHT20 *sensor);
double aht20_read_temperature(AHT20 *sensor);
void aht20_close(AHT20 *sensor);

#endif
