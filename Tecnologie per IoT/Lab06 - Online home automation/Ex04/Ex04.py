import requests
import paho.mqtt.client as PahoMQTT
import paho.mqtt.publish as publish
import json


class Subscriber(object):

    def __init__(self, broker, topic, topics):
        self.clientID = 'tiot23client'
        self._paho_mqtt = PahoMQTT.Client(self.clientID, False)
        self._paho_mqtt.on_connect = self.onConnect
        self._paho_mqtt.on_message = self.onMessageReceived
        self.topic = topic
        self.topics = topics
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

            """
            In un contesto reale i messaggi ricevuti sui topic verrebbero messi a 
            disposizione dell'user, in questo caso questo viene simulato attraverso delle print
            """
            if msg.topic == self.topics[0]:
                print("Temperature: ", data['v'], data['u'])
            elif msg.topic == self.topics[1]:
                print('Presence: ', data['v'])
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
        did = input('Insert desired device id: ')

        arduino = devices[did]
    else:
        print('Some error occurred')

    # la composizione del dizionario è nota perché si ha a disposizione il codice arduino
    topicTemperature = arduino["end-points"]["m"]["t"] + arduino["resources"]["mes"][0]
    topicPresence = arduino["end-points"]["m"]["t"] + arduino["resources"]["mes"][1]
    topicMessage = arduino["end-points"]["m"]["t"] + arduino["resources"]["mes"][0]
    topicTempRange = arduino["end-points"]["m"]["t"] + arduino["resources"]["mes"][1]

    
    subTemperature = Subscriber(broker, topicTemperature, [topicTemperature, topicPresence])
    subPresence = Subscriber(broker, topicPresence, [topicTemperature, topicPresence])
    try:
        subTemperature.startMQTT()
        subPresence.startMQTT()

        # semplice interfaccia tra questo service e un ipotetico user
        while True:
            rep = int(input("Insert 0 = insert message, 1 = change temperature ranges : "))
            if rep == 0:
                message = input('Insert the message (max 32 char): ')
                msg = dict()
                split(message, msg["l1"], msg["l2"])
                publish.single(topicMessage, payload=json.dumps(msg), hostname=broker)
            elif rep == 1:
                temprange = dict()
                temprange["acm"] = input("Insert min ac temperature: ")
                temprange["acM"] = input("Insert min ac temperature: ")
                temprange["htm"] = input("Insert min ht temperature: ")
                temprange["htM"] = input("Insert min ht temperature: ")
                publish.single(topicTempRange, payload=json.dumps(temprange), hostname=broker)
    except:
        subTemperature.stopMQTT()
        subPresence.stopMQTT()
