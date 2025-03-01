# Motiplant - Embedded System with BeagleBone Black

This is a project developed in collaboration with <a href="https://github.com/sfoDavid">David Silva</a> for the discipline of Programming Techniques for Embedded Systems II.
Motiplant is an embedded system project using Linux and BeagleBone Black to monitor and display plant health data. The system reads temperature and soil moisture from sensors and publishes the data via MQTT. Additionally, it renders the information on a 16x2 LCD display and determines an "emotion" for the plant based on soil moisture levels.

## Features
- Reads data from an **AHT20 I2C temperature sensor**.
- Reads soil moisture using a **resistive soil moisture sensor (analog) with an ADS (I2C interface)**.
- Displays data on an **I2C 16x2 LCD**.
- Publishes temperature, soil moisture, and plant emotion via **MQTT (using MQTT Paho)**.
- Configurable **MQTT broker (default: Mosquitto, but any broker should work)**.
- Configurable soil moisture calibration values.

## MQTT Topics
The system publishes the following topics:

- `/motiplant/sensors/temperature` → Temperature data in **Celsius**.
- `/motiplant/sensors/soil` → Soil moisture data in **percentage (e.g., 70, 100, etc.)**.
- `/motiplant/emoti` → Current **emotion** of the plant.

**MQTT QoS**: All topics use **QoS = 1** by default, but this can be adjusted to **QoS = 0** if real-time data loss is not critical.

## Plant Emotion Mapping
The system determines the plant's "emotion" based on soil moisture levels:

| Soil Moisture (%) | Emotion     |
|------------------|------------|
| < 20%           | Very Sad   |
| 20 - 40%        | Sad        |
| 40 - 60%        | Serious    |
| 60 - 80%        | Happy      |
| 80 - 100%       | Very Happy |

## System Flow
1. Reads data from sensors.
2. Determines plant emotion.
3. Publishes data to MQTT.
4. Renders values on the LCD display.

## Configuration
The soil moisture sensor can be calibrated by modifying the **WET_VALUE** (100% moisture) and **DRY_VALUE** (0% moisture) in `include/application/motiplant.h`:

```c
#define WET_VALUE 5000
#define DRY_VALUE 17000
```

Additionally, ensure you configure **MQTT username, password, and broker URL** in `include/communication/mqtt.h`.

## How to Use
- Consume MQTT data using **mobile apps, websites, or any MQTT client**.
- The default MQTT broker is **Mosquitto**, but any MQTT broker should work.

## Suggested Improvements
- **Mobile App**: A mobile application was developed using **MIT App Inventor** to visualize data, but it is not included in this repository. Consider developing a similar app to monitor the plant remotely.

## License
This project is open-source and available for modification and improvement.

