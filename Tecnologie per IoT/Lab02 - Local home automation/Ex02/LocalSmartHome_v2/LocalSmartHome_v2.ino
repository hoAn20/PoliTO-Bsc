#include <math.h>
#include <LiquidCrystal_PCF8574.h>

// sensors and actuators pins
const int PIN_FAN = 5;
const int PIN_TMP = A0;
const int PIN_HT = 11;
const int PIN_PIR = 12;
const int PIN_MIC = 7;  // interrupt
const int PIN_LAMP = 4;

// global constants
const int MAX_PWM_OUTPUT = 255;
const int MIN_PWM_OUTPUT = 0;
const unsigned long MINUTES = 60e03;
const int TEMP_OFFSET_WITH_PEOPLE = 2;

/**
 * NOTE: To reduce time lost in memory allocation and deallocation while executing this sketch in particular
 * in ISRs I decided to declare exclusively global variables.
 */

// general global variables
volatile unsigned long instant;
float temp;
unsigned long now;

// temperature reading constants from datasheet
const long int R0 = 1e05;
const int B = 4275;
const float K_CONV = 273.15;
const float T0 = 25 + K_CONV;
const int VCC = 1023;

// air conditioning variables
float acMinTemp[2];
float acMaxTemp[2];
int fanSpeed;

// heating control variables
float htMinTemp[2];
float htMaxTemp[2];
int heatIntensity;

// presence checker variables
int peoplePresence;
// presence detection using pir sensor variables
unsigned long pirStartTime;
unsigned long pirTimeout;
volatile boolean pirPeoplePresence;

// display management constants and variables
const int TOT_PAGES = 2;
unsigned long displayStartTime;
unsigned long displayTimeout;
int page;
LiquidCrystal_PCF8574 lcd(0x27);

// smart lamp management constants and variables
const int CLAPS_NUM = 2;
const int MIC_TOL = 100;
volatile int noiseEventsCnt;
volatile unsigned long noiseIntervalStartTime;
volatile unsigned long lastClapTime;
unsigned long noiseInterval;
unsigned long lampStartTime;
boolean lampState;


/**
 * Environment temperature reading function.
 * Using the voltage read from PIM_TMP as sensors output this function calculates before the resistor's
 * resistance and then the temperature on which it is exposed. Thise whole process is done using formulas
 * take from the sensor datasheet.
 * Also constants like R0, B and T0 are read on the datasheet.
 * The thermistor's resistance is correlated to temperature in K so here is converted in °C using K_CONV.
 */
float getTemperature()
{
    int v = analogRead(PIN_TMP);
    float R = ((float)VCC / v - 1) * R0;  // relation resistance-voltage
    return (1 / (log(R / R0) / B + 1.0 / T0)) - K_CONV;  // relation temperature-resistance converted in °C
}

/**
 * AC control.
 * Fan speed management based on environmental temperature compared to a minimum and a maximum.
 * Those bounds are saved in an array containing a different values for situations with and without people 
 * in the room.
 * temp < acMinTemp: fan turned off -> fanSpeed = 0
 * acMinTemp <= temp <= acMaxTep: fan speed linearly increasing while the temperature moves from acMinTemp
 *      to acMaxTemp
 *              _MAX
 *             /
 *            /
 *           /
 *      MIN_/
 * temp > acMaxTemp: fan at maximum available speed -> fanSpeed = 255
 */
void airConditioning()
{
    if (temp > acMinTemp[peoplePresence] && temp < acMaxTemp[peoplePresence])
    {
        fanSpeed = (int)(MAX_PWM_OUTPUT * (temp - acMinTemp[peoplePresence]) / (acMaxTemp[peoplePresence] - acMinTemp[peoplePresence]));
        analogWrite(PIN_FAN, fanSpeed);
    }
    else if (temp >= acMaxTemp[peoplePresence] && fanSpeed != MAX_PWM_OUTPUT)
        analogWrite(PIN_FAN, (fanSpeed = MAX_PWM_OUTPUT));
    else if (temp <= acMinTemp[peoplePresence] && fanSpeed != MIN_PWM_OUTPUT)
        analogWrite(PIN_FAN, (fanSpeed = MIN_PWM_OUTPUT));
}

/**
 * Heating control.
 * Voltage given to the led management based on evironmental temperature compared to a minimum and a maximum.
 * Those bounds are saved in an array containing a different values for situations with and without people 
 * in the room.
 * temp < htMinTemp: maximum available voltage -> heatIntensity = 255
 * htMinTemp <= temp <=htMaxTemp: voltage linearly decreasing while the temperature moves from acMinTemp
 *      to acMaxTemp
 *      MAX_
 *          \
 *           \
 *            \
 *             \_MIN
 * temp > htMaxTemp: minimum available volatge -> heatIntensity = 0
 */
void heatingControl()
{
    if (temp >= htMinTemp[peoplePresence] && temp < htMaxTemp[peoplePresence])
    {
        heatIntensity = (int)(MAX_PWM_OUTPUT * (1 - (temp - htMinTemp[peoplePresence]) / (htMaxTemp[peoplePresence] - htMinTemp[peoplePresence])));
        analogWrite(PIN_HT, heatIntensity);
    }
    else if (temp >= htMaxTemp[peoplePresence] && heatIntensity != MIN_PWM_OUTPUT)
        analogWrite(PIN_HT, (heatIntensity = MIN_PWM_OUTPUT));
    else if (temp < htMinTemp[peoplePresence] && heatIntensity != MAX_PWM_OUTPUT)
        analogWrite(PIN_HT, (heatIntensity = MAX_PWM_OUTPUT));
}

/**
 * PIR sensor movement detection.
 * The detection is controlled through polling, this kind of beaviour is conceded by the nature of the sensor
 * that maintains the signal high for a delay time after the detection (minimum delay time is 3s) so there's
 * no need for an interrupt.
 */
void pirMovementDetection()
{
    if (digitalRead(PIN_PIR) == HIGH)
    {
        pirPeoplePresence = true;
        pirStartTime = millis();
    }
}

/**
 * ISR to manage hand claps counter.
 * On sound detection if there's more than the time tollerance between this moment and the last recorded
 * clap it counts as new.
 */
void noiseDetected()
{
    instant = millis(); 
    if (noiseEventsCnt == 0)
    {
        noiseIntervalStartTime = lastClapTime = instant;
        noiseEventsCnt++;
    }
    else if (instant - lastClapTime > MIC_TOL)
    {
        lastClapTime = instant;
        noiseEventsCnt++;
    }
}

/**
 * Switch lamp state.
 * When during the same time interval we record exactly CALPS_NUM hand clap this function switches the
 * state high->low or low/high. For upgradeability the number of recorded claps has to be exactly the same
 * as the constat, this way we could also add functions for different constants.
 */
void smartLampSwitch()
{
    if (now - noiseIntervalStartTime >= noiseInterval)
    {
        if (noiseEventsCnt == CLAPS_NUM)
        {
            lampState = !lampState;
            digitalWrite(PIN_LAMP, lampState);
            lampStartTime = now;
        }
        noiseEventsCnt = 0;
    }
}

/**
 * Check for pir timeout reached.
 * Checks if the imeout related to the pir is over, in this case if the lamp was turned on during the
 * timeout it turns off the lamp. Updtaes the golbal flag used as index in temperature ranges arrays.
 */
void presenceChecker()
{
    pirMovementDetection();  // movement detection using polling
    if (millis() - pirStartTime >= pirTimeout && pirPeoplePresence)
    {
        pirPeoplePresence = false;
        if (lampStartTime < (pirStartTime + pirTimeout))  // for the borderline case (pirStartTime + pirTimeout) < lampStartTime < millis()
            digitalWrite(PIN_LAMP, (lampState = LOW));
    }
    peoplePresence = pirPeoplePresence ? 1 : 0;
}

/**
 * Output infos on the LCD.
 * Updates the page displayed on the LCD every displayTimeout to let the user see all the information.
 * The page that has to be displayed is controlled by a coutner which is updated cyclically.
 */
void displayStatus()
{
    if (now - displayStartTime >= displayTimeout)
    {
        displayStartTime = now;
        page = (++page) % TOT_PAGES;  // cyclic page counter
        lcd.clear();
        lcd.home();
        switch (page)  // using a switch to be easily upgradable
        {
        case 0:
            lcd.print("T:" + String(temp, 1) + " Pres:" + String(peoplePresence));
            lcd.setCursor(0, 1);
            lcd.print("AC:" + String(100 * fanSpeed / MAX_PWM_OUTPUT) + "% HT:" + String(100 * heatIntensity / MAX_PWM_OUTPUT) + "%");
            break;
        case 1:
            lcd.print("AC m:" + String(acMinTemp[peoplePresence], 1) + " M:" + String(acMaxTemp[peoplePresence], 1));
            lcd.setCursor(0, 1);
            lcd.print("HT m:" + String(htMinTemp[peoplePresence], 1) + " M:" + String(htMaxTemp[peoplePresence], 1));
            break;
        }
    }
}

/**
 * Check for temperature update command.
 * Checks if the command choosen to trigger the update function for temperature ranges is received via Serial
 */
void checkCommand()
{
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        Serial.read();
        if (c == 't' || c == 'T')  // choosen command
            updateTemperatureRange();
        else
        {
            Serial.println("Undefined command");
            delay(100);
        }
    }
}

/**
 * Interface to update temperature ranges.
 * Serial interface to update acMinTemp/acMaxTemp and htMinTemp/htMaxTemp for ac and heating control.
 * The version with people in the room is automatically updated using the offset TEMP_OFFSET_WITH_PEOPLE
 */
void updateTemperatureRange()
{
    Serial.setTimeout(10e03);  // waiting time for input
    Serial.print("Insert ac min temperature (actual " + String(acMinTemp[0]) + "): ");
    delay(100);
    acMinTemp[0] = Serial.parseFloat();
    Serial.read();
    acMinTemp[1] = acMinTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    Serial.println(String(acMinTemp[0]) + " / " + String(acMinTemp[1]));
    delay(100);

    Serial.print("Insert ac max temperature (actual " + String(acMaxTemp[0]) + "): ");
    delay(100);
    acMaxTemp[0] = Serial.parseFloat();
    Serial.read();
    acMaxTemp[1] = acMaxTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    Serial.println(String(acMaxTemp[0]) + " / " + String(acMaxTemp[1]));
    delay(100);
    acMaxTemp[1] = acMaxTemp[0] - TEMP_OFFSET_WITH_PEOPLE;

    Serial.print("Insert ht min temperature (actual " + String(htMinTemp[0]) + "): ");
    delay(100);
    htMinTemp[0] = Serial.parseFloat();
    Serial.read();
    htMinTemp[1] = htMinTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
    Serial.println(String(htMinTemp[0]) + " / " + String(htMinTemp[1]));
    delay(100);

    Serial.print("Insert ht max temperature (actual " + String(htMaxTemp[0]) + "): ");
    delay(100);
    htMaxTemp[0] = Serial.parseFloat();
    Serial.read();
    htMaxTemp[1] = htMaxTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
    Serial.println(String(htMaxTemp[0]) + " / " + String(htMaxTemp[1]));
    delay(100);
    Serial.setTimeout(1e03);
}

void setup()
{
    pinMode(PIN_FAN, OUTPUT);
    pinMode(PIN_HT, OUTPUT);
    pinMode(PIN_TMP, INPUT);
    pinMode(PIN_PIR, INPUT);
    pinMode(PIN_MIC, INPUT);
    pinMode(PIN_LAMP, OUTPUT);

    // air conditioning setup
    acMinTemp[0] = 24.0;
    acMaxTemp[0] = 35.0;
    acMinTemp[1] = acMinTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    acMaxTemp[1] = acMaxTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    fanSpeed = MIN_PWM_OUTPUT;
    analogWrite(PIN_FAN, fanSpeed);

    // heating control setup
    htMinTemp[0] = 16.0;
    htMaxTemp[0] = 22.0;
    htMinTemp[1] = htMinTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
    htMaxTemp[1] = htMaxTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
    heatIntensity = MIN_PWM_OUTPUT;
    analogWrite(PIN_HT, heatIntensity);

    // people presence checker setup
    pirTimeout = 30 * MINUTES;
    pirPeoplePresence = false;
    peoplePresence = 0;
    noiseInterval = 10 * MINUTES;
    noiseTimeout = 60 * MINUTES;
    noisePeoplePresence = false;
    noiseEventsCnt = 0;
    noiseEventsTimes[MIN_NOISE_EVENTS_IN_INTERVAL - 1] = -MIC_TOL;  // needed to enter the first time
    attachInterrupt(digitalPinToInterrupt(PIN_MIC), noiseDetected, RISING);
    isArrayFull = false;

    // display
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    page = 1;
    lcd.print("LAB02 Smart home");
    displayStartTime = millis();
    displayTimeout = 2e03;

    // serial monitor setup
    Serial.begin(9600);
    while (!Serial);
}

void loop()
{
    temp = getTemperature();
    now = millis();

    airConditioning();
    heatingControl();
    smartLampSwitch();
    presenceChecker();
    displayStatus();
    checkCommand();
}
