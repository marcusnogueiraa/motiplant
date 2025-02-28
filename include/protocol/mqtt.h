#ifndef MQTT_H
#define MQTT_H

#include <MQTTClient.h>

#define MQTT_BROKER_ADDRESS "tcp://10.0.0.1:1883"
#define CLIENT_ID "bbb_motiplant"

#define SOIL_HUMIDITY_TOPIC "motiplant/sensor/soil"
#define TEMPERATURE_TOPIC "motiplant/sensor/temperature"
#define EMOTION_TOPIC "motiplant/emoti"

#define QOS_1 1
#define TIMEOUT 1000L

#define USERNAME "motiplant"
#define PASSWORD "plant123"

int mqtt_init();
int mqtt_publish_int(int value, const char* topic, int qos);
int mqtt_publish_double(double value, const char* topic, int qos);
int mqtt_publish_string(const char* value, const char* topic, int qos);
void mqtt_cleanup();
int mqtt_disconnect();

#endif
