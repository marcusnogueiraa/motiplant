#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>

#include "application/motiplant.h"
#include "display/lcd1602.h"
#include "sensors/soil_moisture_ads.h"
#include "sensors/aht20.h"
#include "protocol/mqtt.h"

int main() {
    AHT20 aht20;
    SoilMoistureADS soil_moisture;

    init_app(&aht20, &soil_moisture);
    mqtt_init();

    while (true) {
        int humidity = get_humidity_percentage(&soil_moisture);
        double temperature = get_temperature(&aht20);
        const char* emoti_id = get_emoti_id(humidity);  

        mqtt_publish_int(humidity, SOIL_HUMIDITY_TOPIC, QOS_1);
        mqtt_publish_double(temperature, TEMPERATURE_TOPIC, QOS_1);
        mqtt_publish_string(emoti_id, EMOTION_TOPIC, QOS_1);
        
        print_sensor_data(humidity, temperature);
        render_display(humidity, temperature);
        usleep(700000);
    }

    mqtt_cleanup();
    exit_app(&aht20, &soil_moisture);
    return 0;
}
