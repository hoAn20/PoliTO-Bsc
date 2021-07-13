import requests
import paho.mqtt.publish as publish
import json
import time

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

    lindex = int(input('Insert desired command index(led): '))
    ledTopic = arduino['e']['m'] + arduino['r']['cmd'][lindex]

    try:
        while True:
            publish.single(ledTopic, json.dumps({'bn':'Yun','e':[{'n':'led','t':null,'v':1,'u':null}]}), hostname=broker)
            time.sleep(15)
            publish.single(ledTopic, json.dumps({'bn':'Yun','e':[{'n':'led','t':null,'v':0,'u':null}]}), hostname=broker)
            time.sleep(15)
    except:
        print('Terminated')
