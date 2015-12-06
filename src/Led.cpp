#include "Led.h"

#define LED_OFF 0
#define LED_ON  1

Led::Led(uint8_t channel) :
    channel(channel)
{}

void Led::init()
{
    pinMode(channel, OUTPUT);
    digitalWrite(channel, LOW);
}

void Led::on()
{
    digitalWrite(channel, HIGH);
    state = LED_ON;
}

void Led::off()
{
    digitalWrite(channel, LOW);
    state = LED_OFF;
}

void Led::toggle()
{
    if(state == LED_ON) off();
    else on();
}
