// SevenSegmentDisplay.h
#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#include <vector>
#include <Adafruit_MCP23X17.h>

class SevenSegmentDisplay {
private:
    // Pin connections for seven LED strips
    std::vector<int> ledStrip;
    Adafruit_MCP23X17* mcp1;
    Adafruit_MCP23X17* mcp2;

public:
    // Constructor
    SevenSegmentDisplay(const std::vector<int> ledStrip,
                        Adafruit_MCP23X17* mcp1,
                        Adafruit_MCP23X17* mcp2);
    void displayDigit(int digit);
    void setLedStrip(int ledStripNumber, int state);
    void clearDisplay();
};

#endif // SEVENSEGMENTDISPLAY_H
