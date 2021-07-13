import json
import time
import string
import random
import sys
import paho.mqtt.client as PahoMQTT
import paho.mqtt.publish as publish
import requests


class Subscriber(object):

    def __init__(self, topic, broker):
        # clientID è una stringa casuale per garantire con un certo margine di sicurezza di leggere
        # le proprie risposte
        self.clientID = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(10))
        self._paho_mqtt = PahoMQTT.Client(self.clientID, False)
        
        self._paho_mqtt.on_connect = self.onConnect
        self._paho_mqtt.on_message = self.onMessageReceived
        
        self.messageBroker = broker
        self.topic = topic + self.clientID
        
    def start(self):
        self._paho_mqtt.connect(self.messageBroker, 1883)
        self._paho_mqtt.loop_start()
        self._paho_mqtt.subscribe(self.topic, 2)

    def stop(self):
        self._paho_mqtt.unsubscribe(self.topic)
        self._paho_mqtt.loop_stop()
        self._paho_mqtt.disconnect()

    def onConnect(self, paho_mqtt, userdata, flags, rc):
        print('Connected to ', self.messageBroker, ' on topic ', self.topic,' with result code: ', rc)

    def onMessageReceived(self, paho_mqtt, userdata, msg):
        try:
            data = json.loads(msg.payload)
            if 'id' in data:
                if not hasattr(self, 'myId'):
                    print('Added: ', data)
                else:
                    print('Updated: ', data)
                self.myId = data['id']
            else:
                print('Error: ', data['error'])
                self.myId = ''
        except:
            print('Wrong JSON format')



if __name__ == '__main__':
    device = {
        'end-points': {
            'MQTT': '/tiot/23/fake/humidity/sensor/' + ''.join(random.choice(string.ascii_letters) for _ in range(4))
        },
        'resources': ['humidity']
    }

    resp = requests.get('http://localhost:8080/')
    if resp.status_code == 200:
        try:
            info = json.loads(resp.text)
            broker = info['MQTT']['device']['hostname']
            topic = info['MQTT']['device']['topic']
            reply_base_topic = info['MQTT']['device']['replies-topic']

            sub = Subscriber(reply_base_topic, broker)
            topic = topic + sub.clientID

            sub.start()
            time.sleep(5)
            while True:
                publish.single(topic, json.dumps(device), hostname=broker)
                time.sleep(60)
                if sub.myId != '':
                    device['id'] = sub.myId
                else:
                    # Avviene solo in caso di errori
                    if 'id' in device: 
                        del device['id']
            sub.stop()
        except:
            print('Error: ', sys.exc_info()[0])

    else:
        print('Some error occurred: ', resp.status_code)