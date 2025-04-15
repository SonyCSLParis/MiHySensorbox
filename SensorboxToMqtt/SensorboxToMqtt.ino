#include <sstream>
#include <WiFi.h>  
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include "NTPClient.h"
#include <ArduinoMqttClient.h>
#include "wifi-credentials.h"
#include "mqtt-credentials.h"
#include "root-certificate.h"
#include "system-id.h"
#include "AquaponicsKit.h"

Measurements measurements;
AquaponicsKit sensor_box;
WiFiUDP ntp_udp;
NTPClient ntp_client(ntp_udp);
WiFiClientSecure tls;
MqttClient mqtt(tls);

// Function to check if wifi is connected.
bool wifi_isconnected() 
{
        return (WiFi.status() == WL_CONNECTED);
}

// Function to reconnect wifi if it's not connected.
void reconnect_wifi() 
{
        if (!wifi_isconnected()) {
                Serial.println("Reconnecting to wifi");
                WiFi.begin(ssid, password);
                delay(500);
        }
}

void connect_wifi() 
{
        // attempt to connect to WiFi network:
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);

        WiFi.begin(ssid, password);
        int count = 0;
        while (!wifi_isconnected()) {
                // failed, retry
                Serial.print(".");
                if (++count == 72) {
                  Serial.println();
                  count = 0;
                }
                delay(500);
        }

        Serial.println();
        Serial.println("You're connected to the network");
}

void connect_mqtt()
{
        mqtt.setId(system_id);
        mqtt.setUsernamePassword(mqtt_user, mqtt_password);
        Serial.print("Attempting to connect to the MQTT broker: ");
        Serial.println(mqtt_broker);
        
        if (!mqtt.connect(mqtt_broker, mqtt_port)) {
                Serial.print("MQTT connection failed! Error code = ");
                Serial.println(mqtt.connectError());
                Serial.print("failed, rc=");
                Serial.println(" try again in 5 seconds");
                delay(2000);
        } else {
                Serial.println("Connected to the MQTT broker!");
                Serial.println();
                Serial.println("connected so subscribe");
        }
}

void reconnect_mqtt()
{
        while (mqtt.connected() == 0) {
                connect_mqtt();
        }
}

void setup() 
{
        // Initialize serial and wait for port to open
        Serial.begin(115200);
        //while (!Serial) {
        //        ; 
        //}

        // Set ESP32 mode as a station to be connected to wifi network
        WiFi.mode(WIFI_STA);                    

        // Connect to WiFi. Blocks until connected.
        connect_wifi();

        sensor_box.setup();

        Serial.println("Creating TLS connection");
        Serial.println();
        
        tls.setCACert(root_ca);

        if (!tls.connect(mqtt_broker, mqtt_port)) {
                Serial.println("Connection failed!");

        } else {

                Serial.println("TLS connection okay (?)");
                
                connect_mqtt();

                ntp_client.begin();
        }
}

void loop() 
{
        reconnect_wifi();
        update_and_send_measurements();
}

void update_and_send_measurements()
{
        measurements.reset();
        sensor_box.update(measurements);
        if (measurements.has_measurements()) {
                send_measurements();
        }
}

void send_measurements()
{
        std::string s;
        ntp_client.update();
        make_measurements_string(s);
        send_mqtt_message(s);
        Serial.println(s.c_str());
}

void make_measurements_string(std::string& s)
{
        std::stringstream ss;

        ss << "{";
        ss << "\"id\":\"" << system_id << "\",";
        ss << "\"time\":" << ntp_client.getEpochTime();
        if (measurements.has_temperature()) {
                ss << ",\"t\":" << measurements.get_temperature();
        }
        if (measurements.has_ph()) {
                ss << ",\"ph\":" << measurements.get_ph();
        }
        if (measurements.has_dissolved_oxygen()) {
                ss << ",\"do\":" << measurements.get_dissolved_oxygen();
        }
        if (measurements.has_electrical_conductivity()) {
                ss << ",\"ec\":" << measurements.get_electrical_conductivity();
        }
        if (measurements.has_humidity()) {
                ss << ",\"h\":" << measurements.get_humidity();
        }
        if (measurements.has_co2()) {
                ss << ",\"co2\":" << measurements.get_co2();
        }
        if (measurements.has_voltage0()) {
                ss << ",\"v0\":" << measurements.get_voltage0();
        }
        if (measurements.has_voltage1()) {
                ss << ",\"v1\":" << measurements.get_voltage1();
        }
        if (measurements.has_voltage2()) {
                ss << ",\"v2\":" << measurements.get_voltage2();
        }
        ss << "}";
        s = ss.str();
}

void send_mqtt_message(std::string& s)
{
        // send message, the Print interface can be used to set the message contents
        reconnect_mqtt();
        mqtt.beginMessage(mqtt_topic);
        mqtt.print(s.c_str());
        mqtt.endMessage();
}
