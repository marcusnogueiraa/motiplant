#ifndef MOTIPLANT_H
#define MOTIPLANT_H

#define WET_VALUE 5000
#define DRY_VALUE 17000

#include "aht20.h"
#include "soil_moisture_ads.h"
#include "lcd1602.h"

int init_app(AHT20* aht20, SoilMoistureADS* soil_moisture);
int init_sensors(AHT20* aht20, SoilMoistureADS* soil_moisture);
int init_display();
int get_humidity_percentage(SoilMoistureADS* soil_moisture);
double get_temperature(AHT20* aht20);
void render_display(int humidity, double temperature);
void print_sensor_data(int humidity, double temperature);
int close_sensors(AHT20* aht20, SoilMoistureADS* soil_moisture);
void exit_app(AHT20* aht20, SoilMoistureADS* soil_moisture);
const char* get_emoti_id(int humidity);

#endif
