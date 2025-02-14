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
        while (!wifi_isconnected()) {
                // failed, retry
                Serial.print(".");
                delay(500);
        }

        Serial.println();
        Serial.println("You're connected to the network");
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
                
                // Each client must have a unique client ID
                mqtt.setId(system_id);

                // You can provide a username and password for authentication
                mqtt.setUsernamePassword(mqtt_user, mqtt_password);

                Serial.print("Attempting to connect to the MQTT mqtt_broker: ");
                Serial.println(mqtt_broker);

                if (!mqtt.connect(mqtt_broker, mqtt_port)) {
                        Serial.print("MQTT connection failed! Error code = ");
                        Serial.println(mqtt.connectError());

                        while (1);
                }

                Serial.println("You're connected to the MQTT broker!");
                Serial.println();

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
        ss << "}";
        s = ss.str();
}

void send_mqtt_message(std::string& s)
{
        // send message, the Print interface can be used to set the message contents
        mqtt.beginMessage(mqtt_topic);
        mqtt.print(s.c_str());
        mqtt.endMessage();
}
