#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>

#include "protocol/mqtt.h"

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

int mqtt_init() {
    int rc;

    MQTTClient_create(&client, MQTT_BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("[mqtt] connection error, code: %d\n", rc);
        return rc;
    }

    printf("[mqtt] connected to mqtt broker\n");
    return 0;
}

int mqtt_publish_int(int value, const char* topic, int qos) {
    char payload[50];
    snprintf(payload, sizeof(payload), "%d", value);
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = qos;
    pubmsg.retained = 0;

    int rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("[mqtt] error in connection: %d\n", rc);
        return rc;
    }

    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("[mqtt] published message (topic %s): %d\n", topic, value);
    return 0;
}

int mqtt_publish_double(double value, const char* topic, int qos) {
    char payload[50];
    snprintf(payload, sizeof(payload), "%.2lf", value);
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = qos;
    pubmsg.retained = 0;

    int rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("[mqtt] error in connection: %d\n", rc);
        return rc;
    }

    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("[mqtt] published message (topic %s): %.2lf\n", topic, value);
    return 0;
}

int mqtt_publish_string(const char* value, const char* topic, int qos) {
    char payload[50];
    snprintf(payload, sizeof(payload), "%s", value);
    pubmsg.payload = (void*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = qos;
    pubmsg.retained = 0;

    int rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("[mqtt] error in connection: %d\n", rc);
        return rc;
    }

    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("[mqtt] published message (topic %s): %s\n", topic, value);
    return 0;
}

int mqtt_disconnect() {
    int rc = MQTTClient_disconnect(client, 10000);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("[mqtt] disconnect error: %d\n", rc);
        return rc;
    }
    printf("[mqtt] disconnected!\n");
    return 0;
}

void mqtt_cleanup() {
    MQTTClient_destroy(client);
}
