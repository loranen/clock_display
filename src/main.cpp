#include <Arduino.h>

#include <Adafruit_MCP23X17.h>
#include <Wire.h>

#define LED_PIN 7     // MCP23XXX pin LED is attached to

//Adafruit_MCP23X08 mcp;
Adafruit_MCP23X17 mcp;
int i = 0;

void setup() {
    Serial.begin(115200);
    //while (!Serial);
    Serial.println("MCP23xxx Blink Test!");
    Wire.begin(21, 22);

    // uncomment appropriate mcp.begin
    if (!mcp.begin_I2C(0x20, &Wire)) {
    //if (!mcp.begin_SPI(CS_PIN)) {
        Serial.println("Error.");
        while (1);
    }

    // configure pin for output
    mcp.pinMode(LED_PIN, OUTPUT);

    Serial.println("Looping...");
}

void loop() {
    Serial.println(i);

    i = i+1;
    mcp.digitalWrite(LED_PIN, HIGH);
    delay(100);
    mcp.digitalWrite(LED_PIN, LOW);
    delay(100);}
