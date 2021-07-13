#include <math.h>
#include <LiquidCrystal_PCF8574.h>
#include <Bridge.h>
#include <ArduinoJson.h>
#include <Process.h>
#include <MQTTclient.h>

/**
 * LAB HW02 GR23
 * Tecnologie per IoT
 * 
 * @author Cacciatore Mattia(259543) & Cagnasso Pietro(256604)
 * @version 1.0
 */

// sensors and actuators pins
const int PIN_FAN = 5;
const int PIN_TMP = A0;
const int PIN_HT = 11;
const int PIN_PIR = 12;
const int PIN_MIC = 7;  // interrupt
const int PIN_LAMP = 4;

/**
 * NOTA: Ai fini di limitare al minimo problematiche di memory leakage dovute a multiple allocazioni, le
 * variabili messe in gioco in questo codice sono state dichiarate come globali. Ciò ha avuto come conseguenza 
 * l'impossibilità di rendere più leggibili od ordinate alcune sezioni del codice, specie in presenza di
 * operazioni matematiche.
 */

// global constants
const int MAX_PWM_OUTPUT = 255;
const int MIN_PWM_OUTPUT = 0;
const unsigned long MINUTES = 60e03;  // per leggibilità
const int TEMP_OFFSET_WITH_PEOPLE = 2;  // differenza tra le teperature con persone e senza persone nella stanza

// global variables
volatile unsigned long instant;  // istante temporale attuale per ISR
float temp;
unsigned long now;  // istante temporale attuale omogeneo tra tutte le funzioni chiamate
char c; //carattere dove salvare un input seriale

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
unsigned long pirTimeout;  // intervallo validità della presenza di persone senza ulteriori segnali
volatile boolean pirPeoplePresence;  // flag di presenza delle persone legato al sensore PIR
/* RICHIESTE PRIMA DEL PUNTO 9 */
const int MIN_NOISE_EVENTS_IN_INTERVAL = 20;
const int MIC_TOL = 25;  // intervallo di tolleranza
volatile int noiseEventsCnt;
volatile unsigned long noiseEventsTimes[MIN_NOISE_EVENTS_IN_INTERVAL];
volatile boolean isArrayFull;
unsigned long noiseIntervalStartTime;
unsigned long noiseInterval;  // intervallo temporale in cui accettare MIN_NOISE_EVENTS_IN_INTERVAL per segnalare
                                 // presenza di persone
unsigned long noiseStartTime;
unsigned long noiseTimeout;  // intervallo validità della presenza di persone senza ulteriori segnali
boolean noisePeoplePresence;  // flag di presenza delle persone legato al sensore di rumore

// display management constants and variables
const int TOT_PAGES = 2;
unsigned long displayStartTime;
unsigned long displayTimeout;  // tempo dopo il quale cambiare pagina visualizzata
int page;
LiquidCrystal_PCF8574 lcd(0x27);

// gestione versione online
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


/**
 * Lettura della temperatura ambientale.
 * Attraverso la lettura della tensione in uscita dal sensore ci si riconduce alla temperatura attraverso
 * le relazioni che legano tensione e resistenza e successivamente resistenza e temperatura fornite dal
 * datasheet.
 * Le costanti R0, B, T0 sono costanti fornite dal datasheet.
 * La resistenza del termistore è legata alla temperatura in K, va quindi convertita in °C attraverso K_CONV.
 */
float getTemperature()
{
    int v = analogRead(PIN_TMP);
    float R = ((float)VCC / v - 1) * R0;  // relazione tensione-resistenza
    return (1 / (log(R / R0) / B + 1.0 / T0)) - K_CONV;  // relazione resistenza-temperatura poi convertita in °C
}

/*
RICHIESTA PRIMA DEL PUNTO 6
Descrizione sotto
*/
// void airConditioning()
// {
//     if (temp >= acMinTemp && temp < acMaxTemp)
//     {
//         fanSpeed = (int)((temp - acMinTemp) * MAX_PWM_OUTPUT / (acMaxTemp - acMinTemp));
//         analogWrite(PIN_FAN, fanSpeed);
//     }
//     else if (temp >= acMaxTemp && fanSpeed != MAX_PWM_OUTPUT)
//         analogWrite(PIN_FAN, MAX_PWM_OUTPUT);
//     else if (temp < acMinTemp && fanSpeed != MIN_PWM_OUTPUT)
//         analogWrite(PIN_FAN, MIN_PWM_OUTPUT);
// }

/**
 * RICHIESTA DOPO IL PUNTO 6
 * Controllo dell'impianto AC.
 * Gestione della velocità della ventola in funzione del posizionamento della temperatura ambientale rispetto
 * alle temperature acMinTemp e acMaxTemp. Le temperature acMinTemp e acMaxTemp sono salvate in vettori il
 * cui indice è rappresentato dalla variabile peoplePresence che indica 0='senza persone' e 1='con persone'.
 * temp < acMinTemp: ventola spenta
 * acMinTemp <= temp <= acMaxTep: velocità della ventola linearmente cresecente con lo spostamento di temp
 *      da acMinTemp a acMaxTemp
 *              _MAX
 *             /
 *            /
 *           /
 *      MIN_/
 * temp > acMaxTemp: ventola a velocità massima
 */
void airConditioning()
{
    if (temp > acMinTemp[peoplePresence] && temp < acMaxTemp[peoplePresence])
    {
        fanSpeed = (int)(MAX_PWM_OUTPUT * (temp - acMinTemp[peoplePresence]) / (acMaxTemp[peoplePresence] - acMinTemp[peoplePresence]));
                        // relazione tra temp in acMinTemp e acMaxTemp e la velocità tra MIN_PWM (=0) e MAX_PWM (=255)
        analogWrite(PIN_FAN, fanSpeed);
    }
    else if (temp >= acMaxTemp[peoplePresence] && fanSpeed != MAX_PWM_OUTPUT)
        analogWrite(PIN_FAN, (fanSpeed = MAX_PWM_OUTPUT));
    else if (temp <= acMinTemp[peoplePresence] && fanSpeed != MIN_PWM_OUTPUT)
        analogWrite(PIN_FAN, (fanSpeed = MIN_PWM_OUTPUT));
}

/*
RICHIESTA PRIMA DEL PUNTO 6
Descrizione sotto
*/
// void heatingControl()
// {
//   if (temp >= htMinTemp && temp < htMaxTemp)
//  {
//    heatIntensity = (int)(MAX_PWM_OUTPUT * (1 - (temp - htMinTemp) / (htMaxTemp - htMinTemp)));
//    analogWrite(PIN_HT, heatIntensity);
//  }
//  else if (temp >= htMaxTemp && heatIntensity != MIN_PWM_OUTPUT)
//    analogWrite(PIN_HT, MIN_PWM_OUTPUT);
//  else if (temp < htMinTemp && heatIntensity != MAX_PWM_OUTPUT)
//    analogWrite(PIN_HT, MAX_PWM_OUTPUT);
// }

/**
 * RICHIESTA DOPO IL PUNTO 6
 * Controllo dell'impianto di riscaldamento.
 * Gestione della tensione da fornire al led in funzione del posizionamento della temperatura ambientale
 * rispetto alla temperatura htMinTemp e htMaxTemp. Le temperature htMinTemp e htMaxTemp sono salvate
 * in vettori il cui indice è rappresentato dalla variabile peoplePresence che indica 0='senza persone'
 * e 1='con persone'.
 * temp < htMinTemp: tensione massima
 * htMinTemp <= temp <=htMaxTemp: tensione linearmente decrescente con lo spostamenteo di temp da htMinTemp
 *      a htMaxTemp
 *      MAX_
 *          \
 *           \
 *            \
 *             \_MIN
 * temp > htMaxTemp: tensione nulla
 */
void heatingControl()
{
    if (temp >= htMinTemp[peoplePresence] && temp < htMaxTemp[peoplePresence])
    {
        heatIntensity = (int)(MAX_PWM_OUTPUT * (1 - (temp - htMinTemp[peoplePresence]) / (htMaxTemp[peoplePresence] - htMinTemp[peoplePresence])));
                            // relazione tra temp in acMinTemp e acMaxTemp e la tensione in MIN_PWM e MAX_PWM
        analogWrite(PIN_HT, heatIntensity);
    }
    else if (temp >= htMaxTemp[peoplePresence] && heatIntensity != MIN_PWM_OUTPUT)
        analogWrite(PIN_HT, (heatIntensity = MIN_PWM_OUTPUT));
    else if (temp < htMinTemp[peoplePresence] && heatIntensity != MAX_PWM_OUTPUT)
        analogWrite(PIN_HT, (heatIntensity = MAX_PWM_OUTPUT));
}

/**
 * Rilevazione dei movimenti con sensore PIR.
 * Funzione di rilevamento dei movimenti attraverso sensore PIR gestita con polling. Questo tipo di gestione
 * è permessa dalla natura del sensore, infatti con l'impostazione del delay (direttamente sul sensore) si
 * definisce il tempo per cui il valore rimarrà alto dopo una rilevazione di movimento ciò garantisce la
 * possibilità di entrare in questa funzione tramite polling non richiedendo l'uso di interrupt.
 * Nel caso in cui il segnale letto sia alto si segnala la presenza impostando a vero il flag di presenza
 * legato al sensore PIR e aggiornando il tempo di inizio del periodo in cui si può considerare presente
 * qualche persona anche senza ulteriori segnali.
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
 * RICHIESTA PRIMA DEL PUNTO 9
 * Routine di gestione del rileamento di un suono.
 * ISR attivata al rilevamento di un suono da parte del sensore dedicato. Salva il tempo di ogni rilevazione
 * in un vettore circolare. Per slegare maggiormente il comportamento dall'hardware impiegato si è deciso
 * di introdurre un tempo di tolleranza (MIC_TOL) al di sotto del quale due eventi vengono considerati
 * coincidenti.
 */
void noiseDetected()
{
    instant = millis();
    if (instant - noiseEventsTimes[(noiseEventsCnt != 0) ? (noiseEventsCnt - 1) : (MIN_NOISE_EVENTS_IN_INTERVAL - 1)] > MIC_TOL)
    { // adesso   -    ultimo evento registrato    >   intervallo di tolleranza
        noiseEventsTimes[noiseEventsCnt] = instant;
        noiseEventsCnt = (noiseEventsCnt + 1) % MIN_NOISE_EVENTS_IN_INTERVAL;
        if (!isArrayFull && noiseEventsCnt == 0)
            isArrayFull = true;
    }
}

/**
 * RICHIESTA PRIMA DEL PUNTO 9
 * Controllo correttezza intervallo per segnalare presenza attraverso il suono.
 * Controllando che tra l'ultimo suono rilevato e il primo degli ultimi MIN_NOISE_EVENTS_IN_INTERVAL suoni
 * sia trascorso un intervallo minore di noiseInterval ci si accerta che sia stato rilevato un numero 
 * sufficiente di suoni per poter indicare che qualcuno è presente nella stanza impostando a vero il flag
 * di presenza legato al suono e e aggiornando il tempo di inizio del periodo in cui si può considerare
 * presente qualche persona anche senza ulteriori segnali.
 * È importante che l'esecuzione di questa funzione avvenga in un periodo senza interrupt perché nella ISR
 * noiseDetected() vengono modificati i valori nel vettore e il relativo indice.
 */
void noiseCheckInterval()
{
    noInterrupts();
    if (isArrayFull)
    {
        if (noiseEventsTimes[(noiseEventsCnt != 0) ? (noiseEventsCnt - 1) : (MIN_NOISE_EVENTS_IN_INTERVAL - 1)] - noiseEventsTimes[noiseEventsCnt] <= noiseInterval)
        { // ultimo evento registrato  -  (MIN_EVENTS_IN_INTERVAL)esimo evento registrato prima dell'ultimo   <=  durata intervallo
            noisePeoplePresence = true;
            noiseStartTime = noiseEventsTimes[(noiseEventsCnt != 0) ? (noiseEventsCnt - 1) : (MIN_NOISE_EVENTS_IN_INTERVAL - 1)];
        }
    }
    interrupts();
}

/**
 * RICHIESTA PRIMA DEL PUNTO 9
 * Controllo della presenza di persone nella stanza.
 * Controlla se sono scaduti gli intervalli di timeout relativi a sensore PIR e sensore di suono impostando
 * eventualmente i raltivi flag a falso.
 * Aggiorna il valore del flag generale di presenza in base al valore dei due flag specifici con 1 se
 * almeno uno dei due è ancora su vero o 0 altrimenti. Vengono inseriti dei valori interi perché peoplePresence
 * viene usata come indice di un vettore, risulta quindi formalmente più corretto usare un intero.
 */
void presenceChecker()
{
    pirMovementDetection();  // Rilevazione con sensore PIR tramite polling
    noiseCheckInterval();
    if (now - pirStartTime >= pirTimeout)
        pirPeoplePresence = false;
    if (now - noiseStartTime >= noiseTimeout)
        noisePeoplePresence = false;
    peoplePresence = (pirPeoplePresence || noisePeoplePresence) ? 1 : 0;
}

/*
RICHIESTA PRIMA DEL PUNTO 6
Descrizione sotto
*/
// void displayStatus()
// {
//     if (now - displayStartTime >= displayTimeout)
//     {
//         displayStartTime = now;
//         page = (++page) % TOT_PAGES;
//         lcd.clear();
//         lcd.home();
//         switch (page)
//         {
//         case 0:
//             lcd.print("T: " + String(temp, 1) + "Pres: " + String(peoplePresence));
//             lcd.setCursor(0, 1);
//             lcd.print("AC: " + String(100 * fanSpeed / MAX_PWM_OUTPUT) + "% HT: " + String(100 * heatIntensity / MAX_PWM_OUTPUT) + "%");
//             break;
//         case 1:
//             lcd.print("AC m:" + String(acMinTemp, 1) + " M:" + String(acMaxTemp, 1));
//             lcd.setCursor(0, 1);
//             lcd.print("HT m:" + String(htMinTemp, 1) + " M:" + String(htMaxTemp, 1));
//             break;
//         }
//     }
// }

/**
 * Gestisce l'output delle informazioni su schermo LCD. 
 * Ogni pagina resta a schermo un tempo pari a displayTimeout prima di passare alla successiva.
 * L'indice di pagina è gestito dalla variabile page, che può valere massimo TOT_PAGES-1 (in questo
 * caso, TOT_PAGES = 2).
*/
void displayStatus()
{
    if (now - displayStartTime >= displayTimeout)
    {
        displayStartTime = now;
        page = (++page) % TOT_PAGES;  // Aggiornamento ciclico dell'indice di pagina.
        lcd.clear();
        lcd.home();
        switch (page)  // Si sfrutta una struttura a switch per permettere l'eventuale introduzione di 
        {              // una pagina tramite semplice aggiunta di un ulteriore case.
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
 * Verifica la presenza di un comando in input seriale.
 * L'input è rappresentato da un carattere significativo. Se rilevato, avvia la funzione annessa al comando.
*/
void checkCommand()
{
    if (Serial.available() > 0)
    {
        c = Serial.read();
        Serial.read();
        if (c == 't' || c == 'T')  // Il comando scelto per aggiornare i range di temperatura è il carattere 't' o 'T'.
            updateTemperatureRange();
        else
        {
            Serial.println("Undefined command");
            delay(100);
        }
    }
}

void updateTemperatureRange(const String &topic, const String &subtopic, const String &message)
{
    deserializeJson(doc, message);
    acMinTemp[0] = (doc["acm"].as<String>()).toInt();
    acMinTemp[1] = acMinTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    acMaxTemp[0] = (doc["acM"].as<String>()).toInt();
    acMaxTemp[1] = acMaxTemp[0] - TEMP_OFFSET_WITH_PEOPLE;
    htMinTemp[0] = (doc["htm"].as<String>()).toInt();
    htMinTemp[1] = htMinTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
    htMaxTemp[0] = (doc["htM"].as<String>()).toInt();
    htMaxTemp[1] = htMaxTemp[0] + TEMP_OFFSET_WITH_PEOPLE;
}

void printMessageOnLCD(const String &topic, const String &subtopic, const String &message)
{
    deserializeJson(doc, message);
    String line1 = doc["l1"].as<String>();
    String line2 = doc["l2"].as<String>();
    lcd.clear();
    lcd.home();
    lcd.print(line1);
    lcd.setCursor(0,1);
    lcd.print(line2);
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
        doc.clear();
        deserializeJson(doc, json);
        json = "";
    
        p.begin(F("curl"));
        p.addParameter(F("-X"));
        p.addParameter(F("GET"));
        p.addParameter(doc["r"].as<String>());
        p.run();

        if (p.exitValue())
        {
            Serial.print(F("GET at 192.168.1.12:8080/rest error: "));
            Serial.println(p.exitValue());
        } else {
            while(p.available() > 0)
                json.concat((char)p.read());
            doc.clear();
            deserializeJson(doc, json);
            subscriptionUri = doc["d"].as<String>();

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
                Serial.print(F("POST at 192.168.1.12:8080/devices/subscription error: "));
                Serial.println(p.exitValue());
            } else {
                json = "";
                while(p.available() > 0)
                    json.concat((char)p.read());
                deserializeJson(doc, json);
                if (doc["t"] == "id")
                {
                    const char* idLocal = doc["d"];
                    id = String(idLocal);
                    registered = 1;
                    subscriptionUri = subscriptionUri + id;
                    lastUpdateTime = millis();
                    Serial.println(F("Subscription success"));
                } else
                    Serial.println(F("Error while subscribing..."));
            }
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
        Serial.print(F("PUT at 192.168.1.12:8080/devices/ error: "));
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

void defineMyInfo() {
    doc.clear();
    doc["e"]["m"]["t"] = myTopic;
    doc["r"]["mes"][0] = F("temperature");
    doc["r"]["mes"][1] = F("presence");
    doc["r"]["cmd"][0] = F("message");
    doc["r"]["cmd"][1] = F("temprange");
    char myInfoLocal[66];
    serializeJson(doc, myInfoLocal);
    myInfo = myInfoLocal;
}

void defineMyTemperature()
{
    doc.clear();
    doc["v"] = temp;
    doc["u"] = F("Celsius");

    serializeJson(doc, myTemperature);
}

void defineMyPresence()
{
    doc.clear();
    doc["v"] = peoplePresence;

    serializeJson(doc, myPresence);
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
    /* RICHIESTE PRIMA DEL PUNTO 9 */
    noiseInterval = 10 * MINUTES;
    noiseTimeout = 60 * MINUTES;
    noisePeoplePresence = false;
    noiseEventsCnt = 0;
    noiseEventsTimes[MIN_NOISE_EVENTS_IN_INTERVAL - 1] = -MIC_TOL;  // per permettere di entrare immediatamente nell'if in noiseDetected()
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

    // gestione versione online
    digitalWrite(13, LOW);
    Bridge.begin();
    digitalWrite(13, HIGH);
    mqtt.begin(F("test.mosquitto.org"), 1883);
    mqtt.subscribe(myTopic + "temprange", updateTemperatureRange);
    mqtt.subscribe(myTopic + "message", printMessageOnLCD);
    defineMyInfo();
    subscribeToCatalog();
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

     // gestione versione online
     mqtt.monitor()
     if (registered && now - lastUpdateTime > 15e03)
     {
          updateSubscriptionToCatalog();
          defineMyTemperature();
          mqtt.publish(myTopic + "temperature", myTemperature);
          defineMyPresence();
          mqtt.publish(myTopic + "presence", myPresence);
     }

}
