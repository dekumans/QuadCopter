#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "Arduino.h"

class BatteryMonitor {
    public:
        void init();
        void update();
        uint8_t check();
        void print();

    private:
        uint16_t batteryLevels[3];
        uint8_t currentChannel;
};

#endif /*BatteryMonitor.h */
