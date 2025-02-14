import paho.mqtt.client as mqtt
import json
import os

mqtt_broker = os.getenv('MQTT_BROKER')
mqtt_port = int(os.getenv('MQTT_PORT'))
mqtt_topic = os.getenv('MQTT_TOPIC')
mqtt_username = os.getenv('MQTT_USERNAME')
mqtt_password = os.getenv('MQTT_PASSWORD')

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    s = msg.payload.decode("utf-8")
    try:
        data = json.loads(s)
        print(msg.topic + ": " + json.dumps(data, indent=4))
    except ValueError as e:
        print(f"Failed to parse: {s}")
        print(f"Error: {e}")
        
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.tls_set()
client.username_pw_set(username=mqtt_username, password=mqtt_password)
client.connect(mqtt_broker, mqtt_port, 60)
client.loop_forever()

