# MiHySensorbox

The sensor box for the Mi-Hy prototype. It currently uses the [Wi-Fi Aquaponics Kit](https://atlas-scientific.com/product/wi-fi-aquaponics-kit/) from Atlas Scientific.

## SensorboxToMqtt

This directory contains the Arduino code for the Aquaponics Kit to collect the sensor data and broadcast the values over [MQTT](https://en.wikipedia.org/wiki/MQTT). It currently uses [HiveMQ](https://www.hivemq.com) as a message broker. Because it requires an encrypted and authenticated connection you will have to set up an (free) account and (free) server at HiveMQ.

### Installation

To do...


Make sure your Aquaponics Kit works. Please follow the [documentation](https://files.atlas-scientific.com/Wi-Fi-Aquaponics-kit-datasheet.pdf) provided by Altas Scientific to make sure your kit works fine and all the sensors are callibrated. This documentation will guide you also through the installation of some of the required Arduino libraries and how to upload the Arduino code.


Install ArduinoMqttLibrary

Copy and edit the following files (remove the '-EDIT' postfix):
* mqtt-credentials-EDIT.h
* wifi-credentials-EDIT.h
* system-id-EDIT.h


## MqttToConsole

This python script listens to the MQTT broker for the updates from the sensor box (sent via SensorboxToMqtt) and prints out the sensor values to the console.

### Installation

```pip3 install paho-mqtt```

On Linux and Mac, copy my-credentials-EDIT.sh to my-credentials.sh. Then open the new file with a plain text editor and set the correct values for the connection parameters. The Atlas credentials are not needed for this script. 

### Usage

On Linux and Mac

In a terminal window, change to the MqttToConsole directory and then execute the followig command:

```bash mqtt_to_console.sh```



## MqttToSacred

This python script listens to the MQTT broker for the updates from the sensor box (sent via SensorboxToMqtt)and stores the values in a database using [Sacred](https://sacred.readthedocs.io/en/stable/index.html).

### Installation

To do...

```pip3 install paho-mqtt```

Install Sacred: see [https://pypi.org/project/sacred/](https://pypi.org/project/sacred/)

Install MongoDB?

On Linux and Mac, copy my-credentials-EDIT.sh to my-credentials.sh. Then open the new file with a plain text editor and set the correct values for the connection parameters. The Atlas credentials are not needed for this script. 


### Usage

On Linux and Mac

In a terminal window, change to the MqttToConsole directory and then execute the followig command:

```bash mqtt_to_sacred.sh <system-id>```




