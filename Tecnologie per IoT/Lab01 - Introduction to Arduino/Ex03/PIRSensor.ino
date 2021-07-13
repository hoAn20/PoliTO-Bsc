/*
	Write a sketch to count the number of people passing in front of a PIR (Passive InfraRed)
	sensor.	The provided PIR sensor	works with digital pins.

	HC-SR501 datasheet: https://www.epitran.it/ebayDrive/datasheet/44.pdf
*/

const int PIN_LED = 11;
const int PIN_PIR = 7;

volatile int cnt = 0;

void checkObstaclePresence()
{
	volatile int sensorValue = digitalRead(PIN_PIR);  // reads the value from a digital pin (H/L)
	if (sensorValue == HIGH)
	{
		cnt++;
		digitalWrite(PIN_LED, HIGH);
		delay(1e3);
		digitalWrite(PIN_LED, LOW);
	}
}

void setup()
{
	Serial.begin(9600);
	while (!Serial);

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_PIR, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_PIR), checkObstaclePresence, CHANGE);
			// pairs the event of changing state (0->1 or 1->0) on a pin with an interrupt and
			// relative ISR
}

void loop()
{
	delay(30 * 1e03);
	Serial.print("Recorded movements: ");
	Serial.println(cnt);
}
