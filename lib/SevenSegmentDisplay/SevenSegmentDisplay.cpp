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

// Control individual led strip
void SevenSegmentDisplay::setLedStrip(int ledStripNumber, int state) {
    int led = 0;

    if (ledStripNumber <= 15) {
        led = ledStripNumber;
        mcp1->digitalWrite(led, state);
    }
    else if (ledStripNumber > 15) {
        led = ledStripNumber-16;
        mcp2->digitalWrite(led, state);
    }

}