/*
	Write a sketch that modifying the previous one adds a welcome message (e.g. "Lab 1.2) Insert R
	or G to see the actual state of corresponding LED") and implements a function that displays the
	status (low/high) of the LED required from the user using serial input (e.g.'R' for red and 'G'
	for green).
*/

#include <TimerOne.h>

const int LEDR_PIN = 11;
const int LEDG_PIN = 12;
const int LEDR_HALF_T = 1.5;
const int LEDG_HALF_T = 3.5;

volatile int ledrStatus = LOW;
volatile int ledgStatus = LOW;
volatile int byteRead;

void ledgFlash()
{
	ledgStatus = !ledgStatus;
	digitalWrite(LEDG_PIN, ledgStatus);
}

void printStatus()
{
	if (Serial.available() > 0)  // check if the serial buffer isn't empty
	{
		byteRead = Serial.read();  // reads a byte from the buffer
		//Serial.read();  needed if using \r\n line ending mode in serial communications

		if (byteRead == 'R')
		{
			Serial.print("Red led status: ");
			Serial.println(ledrStatus);
		}
		else if (byteRead == 'G')
		{
			Serial.print("Green led status: ");
			Serial.println(ledgStatus);
		}
		else
		{
			Serial.println("Incorrect char.");
		}
	}
}

void setup()
{
	Serial.begin(9600);  // initialization of data transmission over serial port with a 9600 baud
			// rate (bps)
	while (!Serial);  // waits until the serial communication is established
	Serial.println("Lab 1.2)\n Insert R or G to see the actual state of corresponding LED.");
			// prints on serial monitor

	pinMode(PIN_LED_ROSSO, OUTPUT);
	pinMode(PIN_LED_VERDE, OUTPUT);

	Timer1.initialize(LEDG_HALF_T * 1e06);
	Timer1.attachInterrupt(ledgFlash);
}

void loop()
{
	ledrStatus = !ledrStatus;
	digitalWrite(LEDR_PIN, ledrStatus);
	delay(LEDR_HALF_T * 1e03);

	noInterrupts(); // stops interrupts until interrupts(); is executed
	printStatus();
	interrupts();
}
