//#include <avr/interrupt.h>
//#include <stdint.h>

#include "Arduino.h"
#include "BatteryMonitor.h"

void BatteryMonitor::init()
{
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);
}

void BatteryMonitor::update()
{
    uint16_t value = analogRead(batteryChannels[currentChannel]);
    batteryLevels[currentChannel] = value;
    currentChannel++;
    if(currentChannel > 2) {
        currentChannel = 0;
    }
}

uint8_t BatteryMonitor::check()
{
    if(batteryLevels[0] > 750) {
        return 1;
    }
    else {
        return 0;
    }
}

void BatteryMonitor::print()
{
    float convert = (batteryLevels[0] / 1024.0f) * 5.0;
    Serial.print("Battery: ");
    Serial.print(convert, 2);
    Serial.println("V");
    //Serial.print(" ");
    //Serial.print(batteryLevels[1], DEC);
    //Serial.print(" ");
    //Serial.println(batteryLevels[2], DEC);

}
