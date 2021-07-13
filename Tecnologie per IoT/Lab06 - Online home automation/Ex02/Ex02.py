import requests
import paho.mqtt.client as PahoMQTT
import json


class Subscriber(object):

    def __init__(self, broker, topic):
        self.clientID = 'tiot23client'
        self._paho_mqtt = PahoMQTT.Client(self.clientID, False)
        self._paho_mqtt.on_connect = self.onConnect
        self._paho_mqtt.on_message = self.onMessageReceived
        self.topic = topic
        self.messageBroker = broker

    def startMQTT(self):
        self._paho_mqtt.connect(self.messageBroker, 1883)
        self._paho_mqtt.loop_start()
        self._paho_mqtt.subscribe(self.topic, 2)

    def stopMQTT(self):
        self._paho_mqtt.unsubscribe(self.topic)
        self._paho_mqtt.loop_stop()
        self._paho_mqtt.disconnect()
        print('Disconnected from ', self.messageBroker, ' on topic ', self.topic)

    def onConnect(self, paho_mqtt, userdata, flags, rc):
        print('Connected to ', self.messageBroker, ' on topic ', self.topic,' with result code: ', rc)

    def onMessageReceived(self, paho_mqtt, userdata, msg):
        try:
            data = json.loads(msg.payload)

            print('Temperature: ', data['e'][0]['v'], data['e'][0]['u'])
        except ValueError:
            print('Invalid JSON format')


if __name__ == '__main__':
    me = {
        'end-points': {
            'REST': 'rest uri'
        },
        'description': 'reading temperature from arduino yun'
    }

    # recupero nformazioni utili
    info = requests.get('http://192.168.1.12:8080/')
    if info.status_code == 200:
        try:
            accessinfo = json.loads(info.text)

            # per ottenere il broker
            infomqtt = requests.get(accessinfo['m'])
            if infomqtt.status_code == 200:
                mqttaccessinfo = json.loads(infomqtt.text)
                broker = mqttaccessinfo['h']
            else:
                print('Some error occurred')

            # per iscriversi come servizio
            inforest = requests.get(accessinfo['r'])
            if inforest.status_code	== 200:
                restaccessinfo = json.loads(inforest.text)
                # iscrizione come servizio
                resp = requests.post(restaccessinfo['s'], json=me)
                if resp.status_code == 200:
                    data = json.loads(resp.text)
                    me['id'] = data['id']
                else:
                    print('Some error occurred during subscription')
            else:
                print('Some error occurred')
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred')

    devicesinfo = requests.get('http://192.168.1.12:8080/devices')
    if devicesinfo.status_code == 200:
        devices = json.loads(devicesinfo.text)

        """
        In un caso reale sarebbe lo user a indicare quale id usare tra quelli comunicatigli
        dal service, ma in questo contesto viene simulata la scelta attraverso la print e
        la input
        """
        print(devices)
        did = input("Insert desired id: ")

        arduino = devices[did]
    else:
        print('Some error occurred')

    print(arduino)
    tindex = input('Input index of desired measurement(temperature): ')
    temperatureTopic = arduino['e']['m'] + arduino['r']['mes'][tindex]

    sub = Subscriber(broker, temperatureTopic)
    try:
        sub.startMQTT()
        while True:
            pass
    except:
        sub.stopMQTT()
