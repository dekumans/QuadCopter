#ifndef LED_H
#define LED_H

#include "Arduino.h"

class Led
{
    public:
        Led(const uint8_t channel);

        void init();
        void on();
        void off();
        void toggle();

    private:
        const uint8_t channel;
        uint8_t state;
};

#endif /* led.h */
