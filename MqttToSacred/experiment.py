import time    
from datetime import datetime
import sys
import os
import json
from sacred import Experiment
from sacred.observers import FileStorageObserver
from sacred.observers import MongoObserver
import paho.mqtt.client as mqtt

mqtt_broker = os.getenv('MQTT_BROKER')
mqtt_port = int(os.getenv('MQTT_PORT'))
mqtt_topic = os.getenv('MQTT_TOPIC')
mqtt_username = os.getenv('MQTT_USERNAME')
mqtt_password = os.getenv('MQTT_PASSWORD')

ex = Experiment()

sacred_backend = "mongodb"
#sacred_backend = "file"

if sacred_backend == "mongodb":
    mongo_user = os.getenv('ATLAS_USER')
    mongo_password = os.getenv('ATLAS_PASSWORD')
    mongo_url = os.getenv('ATLAS_URL')
    mongo_uri = f"mongodb+srv://{mongo_user}:{mongo_password}@{mongo_url}"
    ex.observers.append(MongoObserver(mongo_uri, 'mihy_data'))
    print(f"Created Mongo Observer at mongodb+srv://{mongo_user}@{mongo_url}")
elif sacred_backend == "file":
    ex.observers.append(FileStorageObserver('mihy_data'))
else:
    raise ValueError(f'Unknown Sacred database back-end: {sacred_backend}. '
                     + f'Known options "mongodb" or "file"')
    
    
def get_date_index():
    now = datetime.now()
    # Record until the end of the day
    return int(now.strftime('%Y%m%d'))
    # Record until the end of the minute
    #return int(now.strftime('%Y%m%d%H%M'))


def finished(date_index):
    return date_index != get_date_index()


def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    client.subscribe(mqtt_topic)


def on_message(client, userdata, msg):
    selected_system = userdata['system-id']
    s = msg.payload.decode("utf-8")
    try:
        data = json.loads(s)
        if data['id'] == selected_system:
            store_data(data)
            print(f"Stored data. Time {data['time']}")
    except ValueError as e:
        print(f"Failed to parse: {s}")
        print(f"Error: {e}")

        
def store_data(data):
    store_metric(data, 'time', 'time')
    store_metric(data, 't', 'temperature')
    store_metric(data, 'h', 'humidity')
    store_metric(data, 'co2', 'co2')
    store_metric(data, 'ph', 'ph')
    store_metric(data, 'ec', 'electrical-conductivity')
    store_metric(data, 'do', 'dissolved-oxygen')

    
def store_metric(data, shortname, longname):
    if shortname in data:
        ex.log_scalar(longname, data[shortname])
    else:
        ex.log_scalar(longname, -sys.float_info.max)


def create_mongo_backend():
    global ex
    mongo_user = os.getenv('ATLAS_USER')
    mongo_password = os.getenv('ATLAS_PASSWORD')
    mongo_url = os.getenv('ATLAS_URL')
    mongo_uri = f"mongodb+srv://{mongo_user}:{mongo_password}@{mongo_url}"
    ex.observers.append(MongoObserver(mongo_uri, 'mihy_data'))
    print(f"Created Mongo Observer at mongodb+srv://{mongo_user}@{mongo_url}")


def create_file_backend():
    global ex
    ex.observers.append(FileStorageObserver('mihy_data'))
    
@ex.automain
def mqtt_experiment_main(_run, _config):
    date_index = get_date_index()
    selected_system = _config['system-id']
    userdata = {
        'system-id': selected_system
    }

    print(f"Recording for date index {date_index} for system {selected_system}.")
    
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2,
                         userdata=userdata)
    client.on_connect = on_connect
    client.on_message = on_message

    client.tls_set()
    client.username_pw_set(username=mqtt_username, password=mqtt_password)
    client.connect(mqtt_broker, mqtt_port, 60)
    
    while not finished(date_index):
        client.loop()
