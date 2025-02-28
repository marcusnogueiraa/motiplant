#ifndef SOIL_MOISTURE_ADS_H
#define SOIL_MOISTURE_ADS_H

#include <stdint.h>

#define ADS1115_ADDRESS 0x48  
#define I2C_BUS "/dev/i2c-2"  

typedef struct {
    int i2c_fd;
    uint8_t channel;
    int wet_value;
    int dry_value;
} SoilMoistureADS;

int soil_moisture_ads_init(SoilMoistureADS *sensor, uint8_t channel, int wet_value, int dry_value);
int soil_moisture_ads_read_raw(SoilMoistureADS *sensor);
int soil_moisture_ads_read_percentage(SoilMoistureADS *sensor);
void soil_moisture_ads_close(SoilMoistureADS *sensor);

#endif

