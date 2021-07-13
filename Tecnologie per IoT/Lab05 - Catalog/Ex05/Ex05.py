import cherrypy as cp
import uuid
import json
import time
import paho.mqtt.client as PahoMQTT
import paho.mqtt.publish as publish
from cherrypy.process.plugins import Monitor


class Catalog(object):
    exposed = True

    def __init__(self):
        # Informazioni di contatto
        self.info = {
            'REST': {
                'device': 'http://192.168.1.12:8080/devices/subscription/',
                'service': 'http://localhost:8080/services/subscription/',
                'user': 'http://localhost:8080/users/subscription/'
            },
            'MQTT': {
                'device': {
                    'hostname': 'test.mosquitto.org',
                    'port': '1883',
                    'topic': '/tiot/23/catalog/devices/subscription/',
                    'replies-topic': '/tiot/23/catalog/devices/reply/subscription/'
                }
            }
        }

        # Strutture dati che contengono i dispositivi, utenti e servizi noti
        self.devices = dict()
        self.services = dict()
        self.users = dict()
        self.devices_removed = list()
        self.services_removed = list()

        # Per la cancellazione automatica passati 2 minuti dall'aggiornamento
        Monitor(cp.engine, self.removeUnusedSubscriptions, frequency=60).subscribe()

        # MQTT subscriber setup
        self.clientID = 'tiot23ca'
        self._paho_mqtt = PahoMQTT.Client(self.clientID, False)
        self._paho_mqtt.on_connect = self.onConnect
        self._paho_mqtt.on_message = self.onMessageReceived
        self.topic = self.info['MQTT']['device']['topic'] + '#'
        self.messageBroker = self.info['MQTT']['device']['hostname']


    def GET(self, *path, **query):
        """
        Restituisce informazioni riguardo il server:

        / : informazioni di contatto del server
        /devices : infromazioni su tutti i dispositivi noti
        /users : informazioni su tutti gli utenti noti
        /services : informazioni su tutti i servizi noti
        /devices/{id} : informazioni sul dispositivo con l'id fornito
        /users/{id} : informazioni sull'utente con l'id fornito
        /services/{id} : informazioni sul servizio con l'id fornito
        """

        # Restituisce le informazioni su come contattare il server
        if len(path) == 0:
            return json.dumps(self.info)

        # Restituisce le liste complete dei dispositivi, servizi o utenti
        elif len(path) == 1:
            if path[0] == 'devices':
                return json.dumps(self.devices)
            elif path[0] == 'services':
                return json.dumps(self.services)
            elif path[0] == 'users':
                return json.dumps(self.users)
            else:
                raise cp.HTTPError(400, 'GET not available at given URI')

        # Restituisce le informazioni su uno specifico dispositivo, servizio o utente identificato
        # dall'ultimo campo nel path
        elif len(path) == 2:
            if path[0] == 'devices' and path[1] in self.devices:
                return json.dumps(self.devices[path[1]])
            elif path[0] == 'services' and path[1] in self.services:
                return json.dumps(self.services[path[1]])
            elif path[0] == 'users' and path[1] in self.users:
                return json.dumps(self.users[path[1]])
            else:
                raise cp.HTTPError(400, 'GET not available at given URI')

        else:
            raise cp.HTTPError(400, 'GET not available at given URI')
    
    def POST(self, *path, **query):
        """
        Per aggiunta di dispositivi, servizi e utenti:

        /devices/subscription : 
            richiede un body con i dati nel seguente formato:
                {
                    'end-points': {
                        'MQTT': {topic},
                        'REST': {URI}  // basta uno solo dei due
                    },
                    'resources': [{resource1}, {resource2}, ...]
                }
        /users/subscription :
            richiede un body con i dati nel seguente formato:
                {
                    'name': {name},
                    'surname': {surname},
                    'email-addresses': [{email1}, {email2}, ...]
                }
        /services/subscription :
            richiede un body con i dati nel seguente formato:
                {
                    'end-points': {
                        'MQTT': {topic},
                        'REST': {URI}  // basta uno solo dei due
                    },
                    'description': {description}
                }
        
        In ognuno dei 3 casi ritorna sempre per risposta ad un successo un body con i dati nel
        seguente formato:
            {
                'id': {id}
            }
        In risposta ad un errore vengono inviati HTTPError della categoria 400.
        """

        body = cp.request.body.read()
        # Controllo di errori nel formato dei JSON ricevuti
        if body == '':
            raise cp.HTTPError(400, 'No data in the received body')
        else:
            try:
                data = json.loads(body)
            except ValueError:
                raise cp.HTTPError(400, 'Wrong JSON format')
        
        if len(path) == 2:
            # Aggiunta di un nuovo dispositivo
            if path[0] == 'devices':
                if path[1] == 'subscription':  # Per permettere l'aggiunta di funzionalità
                    # Controllo errori nel contenuto dei JSON ricevuti
                    if 'end-points' not in data or 'resources' not in data:
                        raise cp.HTTPError(400, 'Wrong device format, end-points and resources needed')
                    if 'REST' not in data['end-points'] and 'MQTT' not in data['end-points']:
                        raise cp.HTTPError(400, 'Wrong device format, REST and/or MQTT end-point needed')

                    udid = self.addDevice(data)
                else:
                    raise cp.HTTPError(400, 'POST not available at given URI')
                return json.dumps({'id': udid})

            # Aggiunta di un nuovo utente
            elif path[0] == 'users':
                if path[1] == 'subscription':  # Per permettere l'aggiunta di funzionalità
                    # Controllo errori nel contenuto dei JSON ricevuti
                    if 'name' not in data or 'surname' not in data or 'email-addresses' not in data:
                        raise cp.HTTPError(400, 'Wrong user format, name and surname and email-addresses needed')

                    uusid = self.addUser(data)
                else:
                    raise cp.HTTPError(400, 'POST not available at given URI')
                return json.dumps({'id': uusid})

            # Aggiunta di un nuovo servizio
            elif path[0] == 'services':
                if path[1] == 'subscription':  # Per permettere l'aggiunta di funzionalità
                    # Controllo errori nel contenuto dei JSON ricevuti
                    if 'description' not in data or 'end-points' not in data:
                        raise cp.HTTPError(400, 'Wrong service format, end-points and description needed')
                    if 'REST' not in data['end-points'] and 'MQTT' not in data['end-points']:
                        raise cp.HTTPError(400, 'Wrong service format, REST and/or MQTT end-point needed')

                    usid = self.addService(data)
                else:
                    raise cp.HTTPError(400, 'POST not available at given URI')
                return json.dumps({'id': usid})
            else:
                raise cp.HTTPError(400, 'POST not available at given URI')
        else:
            raise cp.HTTPError(400, 'POST not available at given URI, too much or too few arguments')

    def PUT(self, *path, **query):
        """
        Per aggiornare dispositivi, servizi e utenti già presenti:
        Gestisce la mancanza dei dispositivi o servizi con l'id fornito aggiungendoli nuovamente,
        la mancanza dell'utente viene gestita con un errore 404

        /devices/subscription/{id} : 
            richiede un body con i dati nello stesso formato della POST, nel caso sia presente nel
            JSON ricevuto la voce id non provoca errori
        /users/subscription/{id} :
            richiede un body con i dati nello stesso formato della POST, nel caso sia presente nel
            JSON ricevuto la voce id non provoca errori
        /services/subscription/{id} :
            richiede un body con i dati nello stesso formato della POST, nel caso sia presente nel
            JSON ricevuto la voce id non provoca errori
        
        Nel caso un dispositivo o servizio sia già stato eliminato, ma il suo sia un id noto, viene
        reinserito nella lista di pertinenza.
        In ognuno dei 3 casi ritorna sempre per risposta ad un successo un body con i dati nel
        seguente formato:
            {
                'id': {id}
            }
        In risposta ad un errore vengono inviati HTTP Error della categoria 400.
        """

        body = cp.request.body.read()
        # Controllo di errori nel formato dei JSON ricevuti
        if body == '':
            raise cp.HTTPError(400, 'No data in the received body')
        else:
            try:
                data = json.loads(body)
            except ValueError:
                raise cp.HTTPError(400, "Wrong JSON format")
        
        if len(path) == 3:
            # Aggiornamento di un dispositivo
            if path[0] == 'devices':
                if path[1] == 'subscription':
                    # Controllo che il dispositivo sia noto (anche già eliminato)
                    if path[2] not in self.devices and path[2] not in self.devices_removed:
                        raise cp.HTTPError(404, 'Unknown device')

                    # Controllo di errori nel contenuto dei JSON ricevuti
                    if 'end-points' not in data or 'resources' not in data:
                        raise cp.HTTPError(400, 'Wrong device format, end-points and resources needed')
                    if 'REST' not in data['end-points'] and 'MQTT' not in data['end-points']:
                        raise cp.HTTPError(400, 'Wrong device format, REST and/or MQTT end-point needed')

                    self.updateDevice(data, path[2])
                    if path[2] in self.devices_removed: self.devices_removed.remove(path[2])
                else:
                    raise cp.HTTPError(400, 'PUT not available at given URI')
                return json.dumps({'id': path[2]})

            # Aggiornamento di un utente
            elif path[0] == 'users':
                if path[1] == 'subscription':
                    # Controllo che l'utente esista
                    if path[2] not in self.users:
                        raise cp.HTTPError(404, 'Unknown user')

                    # Controllo di errori nel contenuto dei JSON ricevuti
                    if 'name' not in data or 'surname' not in data or 'email-addresses' not in data:
                        raise cp.HTTPError(400, 'Wrong user format, name and surname and email-addresses needed')

                    self.updateUser(data, path[2])
                else:
                    raise cp.HTTPError(400, 'PUT not available at given URI')
                return json.dumps({'id': path[2]})

            # Aggiornamento di un servizio
            elif path[0] == 'services':
                if path[1] == 'subscription':
                    # Controllo che il servizio non sia già stato eliminato
                    if path[2] not in self.services and path[2] not in self.services_removed:
                        raise cp.HTTPError(404, 'Unknown service')

                    # Controllo di errori nel contenuto dei JSON ricevuti
                    if 'description' not in data or 'end-points' not in data:
                        raise cp.HTTPError(400, 'Wrong service format, description and end-points needed')
                    if 'REST' not in data['end-points'] and 'MQTT' not in data['end-points']:
                        raise cp.HTTPError(400, 'Wrong service format, REST and/or MQTT end-point needed')

                    self.updateService(data, path[2])
                    if path[2] in self.services_removed: self.services_removed.remove(path[2])
                else:
                    raise cp.HTTPError(400, 'PUT not available at given URI')
                return json.dumps({'id': path[2]})

            else:
                raise cp.HTTPError(400, 'PUT not available at given URI')
        else:
            raise cp.HTTPError(400, 'PUT not available at given URI, too much or too few arguments')           

    def DELETE(self, *path, **query):
        raise cp.HTTPError(400, "DELETE requests aren't available")


    def removeUnusedSubscriptions(self):
        """ Rimozione dei dispositivi o servizi non aggiornati negli ultimi 2 minuti """

        to_del = []  # lista delle chiavi degli elementi da eliminare
        for k in self.devices:
            if time.time() - self.devices[k]['insert-timestamp'] > 120:
                to_del.append(k)
        for k in to_del:
            print('Removed device: ', self.devices[k])
            del self.devices[k]
            self.devices_removed.append(k)

        to_del.clear()
        for k in self.services:
            if time.time() - self.services[k]['insert-timestamp'] > 120:
                to_del.append(k)
        for k in to_del:
            print('Removed service: ', self.services[k])
            del self.services[k]
            self.services_removed.append(k)


    # Metodi per la gestione del MQTT subscriber
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
        """
        Aggiunta dispositivi con richiesta da MQTT:

        Basandosi sulla stringa casuale ultimo parametro del topic di provenienza (.../{random str})
        definisce il topic per la risposta in modo che il dispositivo sia ragionevolmente sicuro che
        sia la risposta alla sua pubblicazione.

        Il messaggio da inviare deve rispettare il formato per nuovi dispositivi: 
            {
                'end-points': {
                    'MQTT': {topic},
                    'REST': {URI}  // basta uno solo dei due
                },
                'resources': [{resource1}, {resource2}, ...]
            }
        Per dispositivi già presenti deve rispettare lo stesso formato con l'aggiunta della voce
        'id': {id}. Nel caso l'id non sia tra quelli noti il dispositivo viene aggiunto di nuovo.

        Per risposte ad un successo si riceverà un messaggio nel formato:
            {
                'id': {id}
            }
        in risposta ad un errore:
            {
                'error': {description}
            }
        """

        identification = msg.topic.split('/')[-1]
        reply_topic = self.info['MQTT']['device']['replies-topic'] + identification

        try:
            data = json.loads(msg.payload)

            # Controllo errori nel contenuto dei JSON ricevuti
            if 'end-points' not in data or 'resources' not in data:
                publish.single(reply_topic, json.dumps({'error':'Wrong device format, end-points and resources needed'}), hostname=self.messageBroker)
                return
            elif 'REST' not in data['end-points'] and 'MQTT' not in data['end-points']:
                publish.single(reply_topic, json.dumps({'error':'Wrong device format, REST and/or MQTT end-point needed'}), hostname=self.messageBroker)
                return
            elif identification not in devices:
                # Aggiunta di un nuovo dispositivo
                publish.single(reply_topic, json.dumps({'id':self.addDevice(data)}), hostname=self.messageBroker)
            else:
                if identification not in self.devices and identification not in self.devices_removed:
                    # Dispositivo ignoto
                    publish.single(reply_topic, json.dumps({'error': 'Unknown device'}), hostname=self.messageBroker)
                else:
                    # Aggiornamento di un dispositivo
                    self.updateDevice(data, identification)
                    if identification in self.devices_removed: self.devices_removed.remove(identification)
                    publish.single(reply_topic, json.dumps({'id':identification}), hostname=self.messageBroker)
        except ValueError:
            # Controllo errori nel formato dei JSON ricevuti
            publish.single(reply_topic, json.dumps({'error':'Wrong JSON format'}), hostname=self.messageBroker)


    # Metodi per aggiunta e aggiornamento di dispositivi, servizi e utenti
    def addDevice(self, data):
        udid = uuid.uuid4()
        self.devices[str(udid)] = {
            'id': str(udid),
            'end-points': data['end-points'],
            'resources': data['resources'],
            'insert-timestamp': time.time()
        }

        print('Added device: ', self.devices[str(udid)])
        return str(udid)

    def addUser(self, data):
        uusid = uuid.uuid4()
        self.users[str(uusid)] = {
            'id': str(uusid),
            'name': data['name'],
            'surname': data['surname'],
            'email-addresses': data['email-addresses']
        }
        
        print('Added user: ', self.users[str(uusid)])
        return str(uusid)

    def addService(self, data):
        usid = uuid.uuid4()
        self.services[str(usid)] = {
            'id': str(usid),
            'end-points': data['end-points'],
            'description': data['description'],
            'insert-timestamp': time.time()
        }

        print('Added service :', self.services[str(usid)])
        return str(usid)

    def updateDevice(self, data, udid):
        self.devices[udid] = {
            'id': udid,
            'end-points': data['end-points'],
            'resources': data['resources'],
            'insert-timestamp': time.time()
        }

        print('Updated device: ', self.devices[udid])

    def updateUser(self, data, uusid):
        self.users[uusid] = {
            'id': uusid,
            'name': data['name'],
            'surname': data['surname'],
            'email-addresses': data['email-addresses']
        }

        print('Updated user: ', self.users[uusid])

    def updateService(self, data, usid):
        self.services[usid] = {
            'id': usid,
            'end-points': data['end-points'],
            'description': data['description'],
            'insert-timestamp': time.time()
        }

        print('Updated service: ', self.services[usid])


if __name__ == "__main__":
    conf = {
        '/': {
            'request.dispatch': cp.dispatch.MethodDispatcher(),
            'tools.sessions.on': True
        }
    }
    catalog = Catalog()
    cp.tree.mount(catalog, '/', conf)

    global_conf = {
        'global': {
            'engine.autoreload.on': False,
            'server.socket_host': '0.0.0.0',
            'server.socket_port': 8080
        }
    }
    cp.config.update(global_conf)

    cp.engine.start()
    try:
        catalog.startMQTT()
        while True:
            pass
    finally:
        catalog.stopMQTT()
        cp.engine.stop()
