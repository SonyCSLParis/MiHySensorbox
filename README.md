# MiHySensorbox

The sensor box for the Mi-Hy prototype. It currently uses the [Wi-Fi Aquaponics Kit](https://atlas-scientific.com/product/wi-fi-aquaponics-kit/) from Atlas Scientific.

## SensorboxToMqtt

This directory contains the Arduino code for the Aquaponics Kit to collect the sensor data and broadcast the values over [MQTT](https://en.wikipedia.org/wiki/MQTT). It currently uses [HiveMQ](https://www.hivemq.com) as a message broker. Because it currently uses an encrypted and authenticated connection you will have to set up an (free) account and (free) server at HiveMQ.

### Installation

To do...


## MqttToConsole

This python script listens to the MQTT broker for the updates from the sensor box (sent via SensorboxToMqtt) and prints out the sensor values to the console.

## MqttToSacred

This python script listens to the MQTT broker for the updates from the sensor box (sent via SensorboxToMqtt)and stores the values in a database using [Sacred](https://sacred.readthedocs.io/en/stable/index.html).

### Installation

To do...





