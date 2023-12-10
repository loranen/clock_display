#include <Arduino.h>

#include <Adafruit_MCP23X17.h>
#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include <vector>
#include <SevenSegmentDisplay.h>
#include "main.h"
#include "secret.h"

#define I2C_SDA	21
#define I2C_SCL 22

int timeStamp = 0;
ErrorCounter errorCnt = {0,0,0};
const int ErrorTimeout = 10;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

// Init io expanders
Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

// Init SSD instanses
std::vector<int> strips1 = { 0,  1,  2,  3,  4,  5,  6};
std::vector<int> strips2 = { 7,  8,  9, 10, 11, 12, 13};
std::vector<int> strips3 = {14, 15, 16, 17, 18, 19, 20};
std::vector<int> strips4 = {21, 22, 23, 24, 25, 26, 27};
SevenSegmentDisplay ledNumber1(strips1, &mcp1, &mcp1);
SevenSegmentDisplay ledNumber2(strips2, &mcp1, &mcp1);
SevenSegmentDisplay ledNumber3(strips3, &mcp1, &mcp1);
SevenSegmentDisplay ledNumber4(strips4, &mcp1, &mcp1);

void get_hours_minutes(int* hours, int* minutes);

void errorMonitor(int errorCode);

void setup() {
    int hours, minutes;

    Serial.begin(115200);
    //while (!Serial);
    Serial.println("MCP23xxx Blink Test!");

    Wire.begin(I2C_SDA, I2C_SCL);

    if (!mcp1.begin_I2C(0x20, &Wire)) {
        errorMonitor(EI2C_GRD);
    }

    // TODO: uncomment when second exapnder is connected
    //if (!mcp2.begin_I2C(0x4E, &Wire)) {
    //    Serial.println("Error I2C init: mcp2.begin_I2C(0x4E, &Wire)");
    //    ESP.restart();
    //}

    // configure pin for output
    for (int i = 0; i < 16; i++) {
        mcp1.pinMode(i, OUTPUT);
    }
    for (int i = 0; i < 16; i++) {
        mcp2.pinMode(i, OUTPUT);
    }

    // Connect to Wi-Fi
    Serial.print("Connecting to ");
    /* These are defined in secret.h
    const char* ssid     = "";
    const char* password = "";
    */
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

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    get_hours_minutes(&hours, &minutes);
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void get_hours_minutes(int* hours, int* minutes) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        errorCnt.Wifi_time_cnt++; // Increment error counter
    }

    *hours = timeinfo.tm_hour;
    *minutes = timeinfo.tm_min;

    return;
}

void loop() {
    int digit1, digit2, digit3, digit4;
    int hours, minutes;

    // Check if error timeout reached
    errorMonitor(EMONITOR);

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


    ledNumber1.displayDigit(digit1);
    ledNumber2.displayDigit(digit2);
    ledNumber3.displayDigit(digit3);
    ledNumber4.displayDigit(digit4);
    delay(1000);
}

void errorMonitor(int errorCode) {
    int performReset = 0;
    switch (errorCode)
    {
    case EWIFI_TIME:
        if (errorCnt.Wifi_time_cnt >= 10)
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
    if (performReset) {
        Serial.println("Error counter timeout reached! -> RESET");
        ESP.restart();
    }
}