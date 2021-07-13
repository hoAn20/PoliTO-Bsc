/*
    Write a sketch to read periodically the ambient temperature. Connect the sensor to the board
    using an analog pin, transform the voltage read from the thermistor using the functions
    available in the datasheet.
*/

#include <math.h>

const int PIN_TMP = A0;
const long int R0 = 100000; // thermistor resistance at nominal temperature 25 (in Ohm)
const int B = 4275;         // non-linear relationin between resistance and temperature (in K)
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV; // nominal temperature (in K)
const int VCC = 1023;         // max readable value in 10-bit system

void setup()
{
    Serial.begin(9600);
    while (!Serial);
    pinMode(PIN_TMP, INPUT);
}

void loop()
{
    volatile int Vsig = analogRead(PIN_TMP);

    // math formula for voltage interpretation as resistance and then temperature
    volatile float R = ((float)VCC / Vsig - 1) * R0;
    volatile float T = (1 / (log(R / R0) / B + 1.0 / T0)) - K_CONV;

    Serial.print("Temperature: ");
    Serial.print(T);
    Serial.println(" °C");
    delay(10e03);
}
