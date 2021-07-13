#include <math.h>
#include <LiquidCrystal_PCF8574.h> // library to control the display

const int TMP_PIN = A0;
const long int R0 = 100000;  // thermistor resistance at nominal temperature 25°C; in kOhm
const int B = 4275;  // non-linear relationin between resistance and temperature; in K
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV;  // nominal temperature; in K
const int VCC = 1023;  // max readable value in 10-bit system

LiquidCrystal_PCF8574 lcd(0x27);  // jumpers A1, A2, A3 on the rear of display's board are
    // disconnected so the address is 0x27

void setup()
{
    lcd.begin(16, 2);  // sets the number of rows and cols
    lcd.setBacklight(255);  // set brightness level
    lcd.home();  // sets cursor in (0,0)
    lcd.clear();  // cleans the display
    lcd.print("Temperature:");

    pinMode(TMP_PIN, INPUT);
}

void loop()
{
    volatile int Vsig = analogRead(TMP_PIN);

    // math formula for voltage interpretation as resistance and then temperature
    volatile float R = ((float)VCC / Vsig - 1) * R0;
    volatile float T = (1 / (log(R / R0) / B + 1.0 / T0)) - K_CONV;

    lcd.setCursor(0, 1);
    String output = (String)T + (char)223 + "C";  // 223 is ASCII code for '°' for our display
    lcd.print(output);

    delay(10e03);
}
