/*
	Write a sketch to control 2 LEDs making them blink with 2 independent periods. To control the
	first LED use the delay() function in the loop function, to control the other LED use an
	interrupt controlled using TimerOne library.
*/

#include <TimerOne.h>  // library for finer PWM control and/or running a periodic interrupt fn

const int LEDR_PIN = 11;
const int LEDG_PIN = 12;
const int LEDR_HALF_T = 1.5;
const int LEDG_HALF_T = 3.5;

volatile int ledrStatus = LOW;  // digital pins have only 2 possible states (HIGH=1, LOW=0)
volatile int ledgStatus = LOW;

void ledgFlash()
{
	ledgStatus = !ledgStatus;
	digitalWrite(LEDG_PIN, ledgStatus);  // applies the state (L/H) to the led
}

void setup()
{
	pinMode(LEDR_PIN, OUTPUT);  // defines pin 11 as an output source
	pinMode(LEDG_PIN, OUTPUT);

	Timer1.initialize(LEDG_HALF_T * 1e06);  // initialization of timer period; uses microseconds
	Timer1.attachInterrupt(ledgFlash);  // ISR that has to be triggered by Timer1 each period
}

void loop()
{
	ledrStatus = !ledrStatus;
	digitalWrite(LEDR_PIN, ledrStatus);
	delay(LEDR_HALF_T * 1e03);  // waiting time; uses milliseconds
}
