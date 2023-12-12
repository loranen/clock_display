#include "SevenSegmentDisplay.h"
#include <vector>

// Constructor
SevenSegmentDisplay::SevenSegmentDisplay(const std::vector<int> ledStrip,
                        Adafruit_MCP23X17* mcp1, Adafruit_MCP23X17* mcp2)
                        : ledStrip(ledStrip), mcp1(mcp1), mcp2(mcp2)
{
}

void SevenSegmentDisplay::displayDigit(int number)
{
  switch (number) {
    case 0:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 0);
      setLedStrip(ledStrip[4], 1);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    case 1:
      setLedStrip(ledStrip[0], 0);
      setLedStrip(ledStrip[1], 0);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 0);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 0);
        break;
    case 2:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 0);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 1);
      setLedStrip(ledStrip[5], 0);
      setLedStrip(ledStrip[6], 1);
        break;
    case 3:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 0);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    case 4:
      setLedStrip(ledStrip[0], 0);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 0);
        break;
    case 5:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 0);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    case 6:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 0);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 1);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    case 7:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 0);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 0);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 0);
        break;
    case 8:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 1);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    case 9:
      setLedStrip(ledStrip[0], 1);
      setLedStrip(ledStrip[1], 1);
      setLedStrip(ledStrip[2], 1);
      setLedStrip(ledStrip[3], 1);
      setLedStrip(ledStrip[4], 0);
      setLedStrip(ledStrip[5], 1);
      setLedStrip(ledStrip[6], 1);
        break;
    default:
        break;
  }
}

// Clear the seven-segment display
void SevenSegmentDisplay::clearDisplay() {
    for (int strip : ledStrip) {
        setLedStrip(strip, 0);
    }
}

/*
MCP23017  Pin Name    Pin ID    PCB
GND        GPA0        0        0
GND        GPA1        1        1
GND        GPA2        2        2
GND        GPA3        3        3
GND        GPA4        4        4
GND        GPA5        5        5
GND        GPA6        6        6
GND        GPA7        7        7
GND        GPB0        8        15
GND        GPB1        9        14
GND        GPB2        10       13
GND        GPB3        11       12
GND        GPB4        12       11
GND        GPB5        13       10
GND        GPB6        14       9
GND        GPB7        15       8

VDD        GPA0        0        24
VDD        GPA1        1        25
VDD        GPA2        2        26
VDD        GPA3        3        27
VDD        GPA4        4        28
VDD        GPA5        5        NC
VDD        GPA6        6        NC
VDD        GPA7        7        NC
VDD        GPB0        8        16
VDD        GPB1        9        17
VDD        GPB2        10       18
VDD        GPB3        11       19
VDD        GPB4        12       20
VDD        GPB5        13       21
VDD        GPB6        14       22
VDD        GPB7        15       23
*/
// Control individual led strip
void SevenSegmentDisplay::setLedStrip(int ledStripNumber, int state) {
    int led = 0;

    // MCP23017 GND
    if (ledStripNumber <= 15) {
        led = MCP_PCB_ID_TO_PIN_ID[ledStripNumber];
        mcp1->digitalWrite(led, state);
    }
    // MCP23017 VDD
    else if (ledStripNumber > 15) {
        led = MCP_PCB_ID_TO_PIN_ID[ledStripNumber];
        mcp2->digitalWrite(led, state);
    }
    //Serial.print("ledStripNumber: ");
    //Serial.println(ledStripNumber);
    //Serial.print("led: ");
    //Serial.println(led);
}