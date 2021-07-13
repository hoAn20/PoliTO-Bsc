#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>
#include <Process.h>

const int PIN_TMP = A0;
const int PIN_LED = 13;

const long int R0 = 100000;
const int B = 4275;
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV;
const int VCC = 1023;

BridgeServer server;
BridgeClient client;

const int capacity = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + 40;
DynamicJsonDocument doc_snd(capacity);

int val;
int errcode;
String url;

/**
 * Lettura della temperatura da sensore.
 * NOTA: onde evitare allocazioni superflue di memoria, la formula di conversione del dato è stata compattata in un'unica istruzione (anche se poco leggibile)
 */
float readTemperature(int pin)
{
	return (1 / (log(((float)VCC / analogRead(pin) - 1)) / B + 1.0 / T0)) - K_CONV;
}

/**
 * Esecuzione della richiesta post con cURL
 */
int postRequest(String data)
{
	Process p;
	p.begin("curl");
	p.addParameter("-H");
	p.addParameter("Content-type: application/json");
	p.addParameter("-X");
	p.addParameter("POST");
	p.addParameter("-d");
	p.addParameter(data);
	p.addParameter(url);
	p.run();

	return p.exitValue();
}

/**
 * Funzione adibita alla formattazione del json da inviare.
 */
String senMLEncode(String res, float v, String unit)
{
	doc_snd.clear();
	doc_snd["bn"] = "Yun";
	doc_snd["e"][0]["n"] = res;
	doc_snd["e"][0]["t"] = millis();
	doc_snd["e"][0]["v"] = v;
	if (unit != "")
		doc_snd["e"][0]["u"] = unit;
	else
		doc_snd["e"][0]["u"] = (char *)NULL;

	String output;
	serializeJson(doc_snd, output);
	return output;
}

void setup()
{
	pinMode(PIN_LED, OUTPUT);

	Serial.begin(9600);
	while(!Serial);
	
	digitalWrite(PIN_LED, LOW);
	Bridge.begin();
	digitalWrite(PIN_LED, HIGH);

	server.listenOnLocalhost();
	server.begin();

	url = "192.168.1.8:8080/log";
}

void loop()
{
  errcode = postRequest(senMLEncode(F("temperature"), readTemperature(PIN_TMP), F("Cel")));
  if (errcode)
		Serial.println("Error: " + String(errcode));

	delay(5e03);
}
