#ifndef REMOTE_H
#define REMOTE_H

#include "Arduino.h"
#include "Filter.h"

#define CHANNELS 4

#define THROTTLE    A8
#define PITCH       A9
#define ROLL        A10
#define YAW         A11
#define AUX1        A12
#define AUX2        A13
#define AUX3        A14
#define AUX4        A15

#define FALLING_EDGE    0
#define RISING_EDGE     1
#define MINONWIDTH      950
#define MAXONWIDTH      2075
#define MINOFFWIDTH     8000
#define MAXOFFWIDTH     24000

struct pinTimingData {
    uint8_t edge;
    uint32_t riseTime;
    uint32_t fallTime;
    uint32_t lastGoodWidth;
};

//const uint8_t channels[CHANNELS] = { THROTTLE, PITCH, ROLL, YAW,
//                                     AUX1, AUX2, AUX3, AUX4 };

const uint8_t channels[CHANNELS] = { THROTTLE, PITCH, ROLL, YAW };

class Remote
{
    public:
        void init();
        void print();
        uint16_t getValue(uint8_t channel);
        void interrupt();

    private:
        volatile struct pinTimingData pinData[CHANNELS];
        volatile uint8_t lastState;

        Filter filteredChannelValues[CHANNELS];
};

#endif /* Remote.h */
