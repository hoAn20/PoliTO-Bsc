#include <Bridge.h>
#include <ArduinoJson.h>
#include <Process.h>
#include <MQTTclient.h>

DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(2) + 100);

const int PIN_LED = 4;

const String pseudoMAC = "AA-BB-CC-00-11-22";
const String myTopic = "/tiot/23/";

String hostname;
String subTopic;
String repTopic;

String myInfo;
String id;

String json;

boolean registered;

/**
 * get a 192.168.1.12:8080/
 *  {
        "r": "192.168.1.12:8080/rest",
        "m": "192.168.1.12:8080/mqtt"
    }
 * get a 192.168.1.12:8080/
 *  {
        "h": "test.mosquitto.org",
        "t": "/tiot/23/catalog/devices/subscription",
        "r": "/tiot/23/catalog/devices/reply/subscription"
    }
 */
void getMQTTInfo()
{
    Process p;
    p.begin(F("curl"));
    p.addParameter(F("-X"));
    p.addParameter(F("GET"));
    p.addParameter(F("192.168.1.12:8080"));
    p.run();

    if (p.exitValue())
    {
        Serial.print(F("GET at 192.168.1.12:8080/ error: "));
        Serial.println(p.exitValue());
    } else {
        while(p.available() > 0)
            json.concat((char)p.read());
        doc.clear();
        deserializeJson(doc, json);
        json = "";
    
        p.begin(F("curl"));
        p.addParameter(F("-X"));
        p.addParameter(F("GET"));
        p.addParameter(doc["m"].as<String>());
        p.run();

        if (p.exitValue())
        {
            Serial.print(F("GET at 192.168.1.12:8080/mqtt error: "));
            Serial.println(p.exitValue());
        } else {
            while(p.available() > 0)
                json.concat((char)p.read());
            doc.clear();
            deserializeJson(doc, json);
            hostname = doc["h"].as<String>();
            subTopic = doc["t"].as<String>();
            repTopic = doc["r"].as<String>();
        }
    }
}

/**
 * informazioni di ritorno
 *  {
        "t": "id"/"error",
        "d": id/desc
    }
 */
void getId(const String &topic, const String &subtopic, const String &message)
{
    DeserializationError err = deserializeJson(doc, message);
    if (err)
    {
        Serial.print(F("Deserialization error: "));
        Serial.println(err.c_str());
    } else {
        if (doc["t"] == "id")
        {
            id = doc["d"].as<String>();
            Serial.println(F("Subscribed correctly"));
            registered = 1;
            mqtt.subscribe(repTopic + id, updateId);
        } else {
            Serial.print(F("Error while subscribing, retrying..."));
            mqtt.publish(subTopic + pseudoMAC, myInfo);
        }
    }
}

/**
 * informazioni da passare
 *  {
        "e": myTopic,
        "r": {
            "mes": ["temperature"],
            "cmd": ["led"]
        }
    }
 * il topic è solo 1 poi in base al contenuto di "r" si aggiungono i subtopic
 * come ad esempio myTopic/temperature
 */
void defineMyInfo() {
    doc.clear();
    doc["e"]["m"] = myTopic;
    doc["r"]["mes"][0] = F("temperature");
    doc["r"]["cmd"][0] = F("led");

    serializeJson(doc, myInfo);
}

/**
 * informazioni di ritorno
 *  {
        "t": "id"/"error",
        "d": id/desc
    }
 */
void updateId(const String &topic, const String &subtopic, const String &message)
{
    DeserializationError err = deserializeJson(doc, message);
    if (err)
    {
        Serial.print(F("Deserialization error: "));
        Serial.println(err.c_str());
    } else {
        if (doc["t"] == "id")
        {
            id = doc["d"].as<String>();
            Serial.println(F("Updated correctly"));
        } else {
            Serial.print(F("Error while updating, retrying..."));
            mqtt.publish(subTopic + id, myInfo);
        }
    }
}

void switchLed(const String &topic, const String &subtopic, const String &message)
{
    deserializeJson(doc, message);

    int value = (doc["e"][0]["v"].as<String>()).toInt();

    digitalWrite(PIN_LED, value);
}

void setup()
{
    pinMode(PIN_LED, OUTPUT);

    Serial.begin(9600);
    while(!Serial);

    digitalWrite(13, LOW);
  	Bridge.begin();
  	digitalWrite(13, HIGH);
   
    // lettura dal catalog degli end-points
    getMQTTInfo();

    // avvio mqtt
    mqtt.begin(hostname, 1883);
    // sottoscrizione al topic di risposta
    mqtt.subscribe(repTopic + pseudoMAC, getId);
    //sottoscrizione al topic di comando
    mqtt.subscribe(myTopic + "led", switchLed);
    // pubblicazione per registrazione al catalog
    defineMyInfo();
    registered = 0;
    mqtt.publish(subTopic + pseudoMAC, myInfo);
}

void loop()
{
    mqtt.monitor();
    if (registered)
    {
        // pubblicazione per aggiornamento sul catalog
        Serial.println(myInfo);
        Serial.println(mqtt.publish(subTopic + id, myInfo));
        
        delay(60e03);
    }
}
