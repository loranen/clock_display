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
    int MCP_PCB_ID_TO_PIN_ID[32] = {
        0,  1,  2,  3,  4,  5,  6, 7,
        8,  9,  10, 11, 12, 13, 14, 15,
        8,  9,  10, 11, 12, 13, 14, 15,
        0,  1,  2,  3,  4,  5,  6, 7
    };


public:
    // Constructor
    SevenSegmentDisplay(const std::vector<int> ledStrip,
                        Adafruit_MCP23X17* mcp1,
                        Adafruit_MCP23X17* mcp2);
    void displayDigit(int digit);
    void clearDisplay();
    void setLedStrip(int ledStripNumber, int state);
};

#endif // SEVENSEGMENTDISPLAY_H
