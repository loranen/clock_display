#include <Arduino.h>

#include <Adafruit_MCP23X17.h>
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include <vector>
#include <SevenSegmentDisplay.h>
#include "main.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define DS_POWER 4
#define I2C_SDA	21
#define I2C_SCL 22

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

int timeStamp = 0;
ErrorCounter errorCnt = {0,0,0};
int WifiErrorTimeout = 10;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

// Init io expanders
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

// Init SSD instanses
//std::vector<int> strips1 = { 0,  1,  2,  3,  4,  5,  6};
//std::vector<int> strips2 = { 7,  8,  9, 10, 11, 12, 13};
//std::vector<int> strips3 = {14, 15, 16, 17, 18, 19, 20};
//std::vector<int> strips4 = {21, 22, 23, 24, 25, 26, 27};

std::vector<int> strips1 = { 27,  26,  25,  24,  23,  22,  21};
std::vector<int> strips2 = { 20,  19,  18, 17, 16, 15, 14};
std::vector<int> strips3 = {13, 12, 11, 10,  9,  8, 7};
std::vector<int> strips4 = { 6,  5,  4,  3,  2,  1, 0};

SevenSegmentDisplay ledNumber1(strips1, &mcp1, &mcp2);
SevenSegmentDisplay ledNumber2(strips2, &mcp1, &mcp2);
SevenSegmentDisplay ledNumber3(strips3, &mcp1, &mcp2);
SevenSegmentDisplay ledNumber4(strips4, &mcp1, &mcp2);

void get_hours_minutes(int* hours, int* minutes);

void errorMonitor(int errorCode);

void turnAllOff();

float getTemperature();

void setup() {
    int hours, minutes;

    pinMode(DS_POWER, OUTPUT); //Set pin 13 as an 'output' pin as we will make it output a voltage.
    digitalWrite(DS_POWER, HIGH);

    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);

    if (!mcp1.begin_I2C(0x20, &Wire)) {
        errorMonitor(EI2C_GRD);
    }

    if (!mcp2.begin_I2C(0x27, &Wire)) {
        errorMonitor(EI2C_VDD);
    }

    // configure pin for output
    for (int i = 0; i < 16; i++) {
        mcp1.pinMode(i, OUTPUT);
    }
    for (int i = 0; i < 16; i++) {
        mcp2.pinMode(i, OUTPUT);
    }

    // Connect to Wi-Fi
    Serial.print("Connecting to ");

    /************************************/
    /*     Change the credentials       */
    /************************************/
    const char* ssid     = "WIFI_NAME";
    const char* password = "WIFI_PASSWORD";

    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        errorCnt.Wifi_time_cnt++;
        errorMonitor(EWIFI_TIME);
    }
    Serial.println("");
    Serial.println("WiFi connected.");

    // Change from 10 to 3: Longer timeput needen in init
    WifiErrorTimeout = 3;
    errorCnt.Wifi_time_cnt = 0;

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    get_hours_minutes(&hours, &minutes);
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void setColon(int state) {
    mcp2.digitalWrite(4, state);
}

void setMinus(int state) {
    mcp2.digitalWrite(9, state);
}

void turnAllOff() {
    int led;

    // MCP23017 GND
    for (int ledStripNumber = 0; ledStripNumber < 16; ledStripNumber++) {
        mcp1.digitalWrite(ledStripNumber, 0);
    }
   // MCP23017 VDD
    for (int ledStripNumber = 0; ledStripNumber < 16; ledStripNumber++) {
        mcp2.digitalWrite(ledStripNumber, 0);
    }
}

void get_hours_minutes(int* hours, int* minutes) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.print("Failed to obtain time ");
        errorCnt.Wifi_time_cnt++; // Increment error counter
        Serial.println(errorCnt.Wifi_time_cnt);

        errorMonitor(EWIFI_TIME);
    }

    *hours = timeinfo.tm_hour;
    *minutes = timeinfo.tm_min;
}

int countDigit(short number) {
   int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}

void loop() {
    int digit1, digit2, digit3, digit4;
    int hours, minutes;
    float raw_temperature;
    short rounded_temperature;

    // Check if error timeout reached
    errorMonitor(EMONITOR);
    Serial.print("Timestamp: ");
    Serial.println(timeStamp);
    timeStamp = timeStamp+1;

    get_hours_minutes(&hours, &minutes);

    digit1 = hours/10;
    digit2 = hours%10;
    digit3 = minutes/10;
    digit4 = minutes%10;

    Serial.print("Digit1: ");
    Serial.println(digit1);
    Serial.print("Digit2: ");
    Serial.println(digit2);
    Serial.print("Digit3: ");
    Serial.println(digit3);
    Serial.print("Digit4: ");
    Serial.println(digit4);

    // Restart to sync clock
    if ((hours == 3) && (minutes == 30))
    {
        Serial.println("Time to reboot to sync clock");
        ESP.restart();
    }

    //turnAllOff();

    ledNumber1.displayDigit(digit1);
    ledNumber2.displayDigit(digit2);
    ledNumber3.displayDigit(digit3);
    ledNumber4.displayDigit(digit4);
    setColon(1);

    delay(5000);

    raw_temperature = getTemperature();
    raw_temperature = raw_temperature + 0.5 - (raw_temperature<0);
    rounded_temperature = (short)raw_temperature;
    if (rounded_temperature == -127)
    {
        Serial.println("Temperature read failed!");
    }
    else{
        turnAllOff();

        if (raw_temperature < 0) {
            Serial.println("Negative temperature");
            setMinus(1);
        }

        rounded_temperature = std::abs(rounded_temperature);

        digit1 = rounded_temperature/10;
        digit2 = rounded_temperature%10;

        Serial.print("rounded_temperature: ");
        Serial.println(rounded_temperature);


        Serial.print("Temp digit1: ");
        Serial.println(digit1);
        Serial.print("Temp digit2: ");
        Serial.println(digit2);

        if (countDigit(rounded_temperature) == 2) {
            ledNumber3.displayDigit(digit1);
            ledNumber4.displayDigit(digit2);
        }
        else {
            ledNumber3.displayDigit(digit2);
        }

        delay(5000);
    }

}

float getTemperature() {
    float temperature;
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);

    Serial.print("Celsius temperature: ");
    Serial.println(temperature);
    return temperature;
}

void errorMonitor(int errorCode) {
    int performReset = 0;
    switch (errorCode)
    {
    case EWIFI_TIME:
        if (errorCnt.Wifi_time_cnt >= WifiErrorTimeout)
        {
            Serial.print("Could not connect to wifi");
            performReset = 1;
        }
        break;
    case EI2C_GRD:
        Serial.println("Error I2C init: MCP23017 GRD");
        performReset = 1;
        break;
    case EI2C_VDD:
        Serial.println("Error I2C init: MCP23017 VDD");
        performReset = 1;
        break;
    default:
        break;
    }

    // If Error counter has been reached
    if (performReset == 1) {
        Serial.println("Error counter timeout reached! -> RESET");
        ESP.restart();
    }
}

