#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <ArduinoJson.h>

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

/**
 * Lettura della temperatura da sensore.
 * NOTA: onde evitare allocazioni superflue di memoria, la formula di conversione del dato è stata compattata in un'unica istruzione (anche se poco leggibile)
 */
float readTemperature(int pin)
{
	return (1 / (log(((float)VCC / analogRead(pin) - 1)) / B + 1.0 / T0)) - K_CONV;
}

/**
 * Composizione della risposta alla richiesta ricevuta con aggiunta di codice per garantire una
 * migliore formattazione.
 */
void printResponse(BridgeClient client, int code, String body)
{
	client.println("Status: " + String(code));

	if (code == 200)
	{
		client.println(F("Content-type: application/json; charset=utf-8"));
		client.println();
		client.println(body);
	}
	else
	{
		client.println(F("Content-type: text/plain; charset=utf-8"));
		client.println();
		client.println(body);
	}
}

/**
 * Funzione adibita alla formattazione del json da inviare.
 */
String senMLEncode(String res, float v, String unit)
{
	String output;

	Serial.println(v);

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

/**
 * Funzione di processamento della richiesta ricevuta gestendo led, temperature ed eventuali errori
 */
void process(BridgeClient client)
{
	String command = client.readStringUntil('/');
	command.trim();
	if (command == "led")
	{
		val = client.parseInt();
		if (val == 0 || val == 1)
		{
			digitalWrite(PIN_LED, val);
			printResponse(client, 200, senMLEncode(F("led"), val, F("")));
		}
		else
		{
			printResponse(client, 400, F("Invalid value for led."));
		}
	}
	else if (command == "temperature")
	{
    // viene ignorato il caso in cui dopo temperature ci siano altre informazioni
    // es. arduino.local/arduino/temperature/23
		printResponse(client, 200, senMLEncode(F("temperature"), readTemperature(PIN_TMP), F("Cel")));
	}
	else
	{
		printResponse(client, 400, F("Invalid request."));
	}
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
}

void loop()
{
	client = server.accept();

	if (client)
	{
		process(client);
		client.stop();
	}

	delay(50);
}
