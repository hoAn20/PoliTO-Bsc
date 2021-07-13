#include <Bridge.h>
#include <ArduinoJson.h>
#include <Process.h>
#include <MQTTclient.h>

DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(2) + 100);

const int PIN_TMP = A0;
const long int R0 = 1e05;
const int B = 4275;
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV;
const int VCC = 1023;

const String pseudoMAC = "AA-BB-CC-00-11-22";
const String myTopic = "/tiot/23/";

String hostname;
String subTopic;
String repTopic;

String myInfo;
String id;
String myTemperature;

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
//            for (int i = 0; i < 8; i++)
//                id.concat(message[18+i]);
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

float getTemperature()
{
    int v = analogRead(PIN_TMP);
    float R = ((float)VCC / v - 1) * R0;  // relazione tensione-resistenza
    return (1 / (log(R / R0) / B + 1.0 / T0)) - K_CONV;  // relazione resistenza-temperatura poi convertita in °C
}

void defineMyTemperature() {
    doc.clear()
    doc["bn"] = "Yun";
    doc["e"][0]["t"] = millis();
    doc["e"][0]["n"] = "temperature"
    doc["e"][0]["v"] = getTemperature();
    doc["e"][0]["u"] = "Cel";

    serializeJson(doc, myTemperature);
}

void setup()
{
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

        // pubblicazione della temperatura
        defineMyTemperature();
        mqtt.publish(myTopic+"temperature", myTemperature);
        
        delay(60e03);
    }
}
