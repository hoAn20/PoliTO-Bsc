/**
 * GIÀ IN QUESTO ESERCIZIO SI SONO PRESENTATI MOLTI ERRORI CON 
 * SRADUINOJSON, BRIDGE E PROCESS
 * 
 */

#include <Bridge.h>
#include <ArduinoJson.h>
#include <Process.h>

DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + JSON_ARRAY_SIZE(2) + 100);
const String myTopic = "/tiot/23/";
String subscriptionUri;
String myInfo;
String id;
String myTemperature;
String myPresence;
String json;
boolean registered;
unsigned long lastUpdateTime;

void defineMyInfo() {
    doc.clear();
    doc["end-points"]["MQTT"]["topic"] = myTopic;
    doc["resources"]["measurement"][0] = F("temperature");
    char myInfoLocal[66];
    serializeJson(doc, myInfoLocal);
    myInfo = myInfoLocal;
}

void subscribeToCatalog()
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
        deserializeJson(doc, json);
        subscriptionUri = doc["REST"]["device"].as<String>();
    
        p.begin("curl");
        p.addParameter(F("-X"));
        p.addParameter(F("POST"));
        p.addParameter(F("-H"));
        p.addParameter(F("Content-type: application/json"));
        p.addParameter(F("-d"));
        p.addParameter(myInfo);
        p.addParameter(subscriptionUri);
        p.run();

        if (p.exitValue())
        {
            Serial.println(F("POST error"));
        } else {
            while(p.available() > 0)
                json.concat((char)p.read());
            deserializeJson(doc, json);
            id = doc["id"].as<String>();
            lastUpdateTime = millis();
            registered = 1;
            Serial.println(F("Subscribe success"));
        }
    }
}

void updateSubscriptionToCatalog()
{
    Process p;
    p.begin("curl");
    p.addParameter(F("-X"));
    p.addParameter(F("PUT"));
    p.addParameter(F("-H"));
    p.addParameter(F("Content-type: application/json"));
    p.addParameter(F("-d"));
    p.addParameter(myInfo);
    p.addParameter(subscriptionUri);
    p.run();

    if (p.exitValue())
    {
        Serial.print(F("PUT at 192.168.1.12:8080/devices/subscription/id error: "));
        Serial.println(p.exitValue());
    } else {
        json = "";
        while(p.available() > 0)
            json.concat((char)p.read());
        deserializeJson(doc, json);
        if (doc["t"] == "id")
        {
            id = "";
            for (int i = 0; i < 36; i++)
                id.concat(json[18+i]);
            lastUpdateTime = millis();
            Serial.println(F("Update success"));
        } else {
            Serial.println(F("Error while updating..."));
        }
    }
}

void setup()
{
  	Serial.begin(9600);
  	while(!Serial);
  
  	digitalWrite(13, LOW);
    Bridge.begin();
    digitalWrite(13, HIGH);

    registered = 0;

    defineMyInfo();
    subscribeToCatalog();
}

void loop()
{
	unsigned long now = millis();

	if (registered && now - lastUpdateTime > 30e03)
	{
		updateSubscriptionToCatalog();
	}
}
