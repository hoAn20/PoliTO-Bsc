#include <MQTTclient.h>
#include <ArduinoJson.h>

const int capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
DynamicJsonDocument doc_rec(capacity);
DynamicJsonDocument doc_snd(capacity);

const int PIN_LED = 13;
const int PIN_TMP = A0;

const long int R0 = 100000;
const int B = 4275;
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV;
const int VCC = 1023;

String baseTopic = "/tiot/23";

/**
 * Deserializzazione del json ricevuto e relativa accensione del led in base al valore ricevuto
 */
void setLedValue(const String &topic, const String &subtopic, const String &message)
{
	DeserializationError err = deserializeJson(doc_rec, message);
	if (err)
	{
		Serial.print(F("deserializedJson() failed with code: "));
		Serial.println(err.c_str());
	}
	if (doc_rec["e"][0]["n"] == "led")
	{
		if (doc_rec["e"][0]["v"] == 0 || doc_rec["e"][0]["v"] == 1)
			digitalWrite(PIN_LED, doc_rec["e"][0]["v"]);
		else
			Serial.println("ERROR: Wrong value received.");
	}
	else
		Serial.println("ERROR: Unknown command, must be 'led'.");
}

/**
 * Lettura della temperatura da sensore.
 * NOTA: onde evitare allocazioni superflue di memoria, la formula di conversione del dato è stata compattata in un'unica istruzione (anche se poco leggibile)
 */
float readTemperature(int pin)
{
	return (1 / (log(((float)VCC / analogRead(pin) - 1)) / B + 1.0 / T0)) - K_CONV;
}

/**
 * Funzione adibita alla formattazione del json da inviare.
 */
String senMLEncode(String res, float v, String unit)
{
	String output;

	doc_snd.clear();
	doc_snd["bn"] = "Yun";
	doc_snd["e"][0]["n"] = res;
	doc_snd["e"][0]["t"] = millis();
	doc_snd["e"][0]["v"] = v;
	if (unit != "")
		doc_snd["e"][0]["u"] = unit;
	else
		doc_snd["e"][0]["u"] = (char *)NULL;

	serializeJson(doc_snd, output);
	return output;
}

void setup()
{
	pinMode(PIN_LED, OUTPUT);

	digitalWrite(PIN_LED, HIGH);
	Bridge.begin();
	digitalWrite(PIN_LED, LOW);

	Serial.begin(9600);
	while(!Serial);

	mqtt.begin("test.mosquitto.org", 1883);
	mqtt.subscribe(baseTopic + String("/led"), setLedValue);
}

void loop()
{
	mqtt.monitor();

	Serial.println(mqtt.publish(baseTopic + String("/temperature"), senMLEncode("temperature", readTemperature(PIN_TMP), "Cel")));

	delay(5e03);
}
