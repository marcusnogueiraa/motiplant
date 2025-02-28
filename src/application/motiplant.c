#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>

#include "application/motiplant.h"
#include "sensors/soil_moisture_ads.h"
#include "sensors/aht20.h"

int init_app(AHT20* aht20, SoilMoistureADS* soil_moisture){
    if (init_sensors(aht20, soil_moisture) != 0) {
        return -1;
    }

    if (init_display() != 0) {
        close_sensors(aht20, soil_moisture);
        return -1;
    }

    return 0;
}

int init_sensors(AHT20* aht20, SoilMoistureADS* soil_moisture) {
    if (aht20_init(aht20) != 0) {
        return -1;
    }

    if (soil_moisture_ads_init(soil_moisture, 0, WET_VALUE, DRY_VALUE) != 0) {
        aht20_close(aht20);
        return -1;
    }

    return 0;

}

int init_display() {
    if (lcd1602_open() != 0) {
        return -1;
    }

    lcd1602_init();
    lcd1602_clear();
    return 0;
}

int get_humidity_percentage(SoilMoistureADS* soil_moisture){
    int humidity = soil_moisture_ads_read_percentage(soil_moisture);
    return humidity;
}

double get_temperature(AHT20* aht20){
    double temperature = aht20_read_temperature(aht20);
    if (temperature == -1000.0) {
        printf("[aht20] erro ao ler dados do sensor.\n");
        return -1000.0;
    }
    return temperature;
}

const char* get_emoti_id(int humidity){
    if (humidity <= 20) return "very_sad"; // very sad (0-20)
    else if (humidity <= 40) return "sad"; // sad (21-40)
    else if (humidity <= 60) return "serious"; // serious (41-60)
    else if (humidity <= 80) return "happy"; // happy (61-80)
    else return "very_happy"; // very vappy (81-100)
}

void render_display(int humidity, double temperature){
    char line1[17], line2[17];
    snprintf(line1, sizeof(line1), "Humidity: %d%%", humidity);
    snprintf(line2, sizeof(line2), "Temp: %.1fC", temperature);

    lcd1602_clear();
    lcd1602_setCursorPosition(0, 0);
    lcd1602_sendString(line1);
    lcd1602_setCursorPosition(1, 0);
    lcd1602_sendString(line2);
}

void print_sensor_data(int humidity, double temperature){
    printf("Humidity: %d%% - ", humidity);
    printf("Temperature: %.1lfC\n", temperature);
}

int close_sensors(AHT20* aht20, SoilMoistureADS* soil_moisture) {
    aht20_close(aht20);
    soil_moisture_ads_close(soil_moisture);
    return 0;
}

void exit_app(AHT20* aht20, SoilMoistureADS* soil_moisture){
    close_sensors(aht20, soil_moisture);
    lcd1602_close();
}