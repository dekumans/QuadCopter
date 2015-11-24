#include <avr/interrupt.h>
#include <stdint.h>

#include "Arduino.h"
#include "Led.h"

#define LED_AMOUNT      3

#define LED_OFF         0
#define LED_ON          1

#define NOT_BLINKING    0
#define BLINKING        1

struct led {
    uint8_t channel;
    uint8_t state;
    uint8_t blinking;
};

static struct led leds[LED_AMOUNT];

void initLeds()
{
    int i;

    leds[0].channel = 13;
    leds[1].channel = 0;
    leds[2].channel = 0;

    for(i = 0; i < LED_AMOUNT; ++i) {
        pinMode(leds[i].channel, OUTPUT);
        digitalWrite(leds[i].channel, LOW);
    }

    //timer init
    //interrupt every 200ms
    TCCR3A  = 0x00;
    TCCR3B  = 0x03; //prescaler 64
    TCCR3C  = 0x00;
    TIMSK3  = 0x01; //enable timer3 ov interrupt
}

void ledOn(const uint8_t led_index)
{
    digitalWrite(leds[led_index].channel, HIGH);
    leds[led_index].state = LED_ON;
}

void ledOff(const uint8_t led_index)
{
    digitalWrite(leds[led_index].channel, LOW);
    leds[led_index].state = LED_OFF;
}

void ledToggle(const uint8_t led_index)
{
    if(leds[led_index].state == LED_ON) {
        ledOff(led_index);
    }
    else {
        ledOn(led_index);
    }
}

void startBlinking(const uint8_t led_index)
{
    leds[led_index].blinking = BLINKING;
}

void stopBlinking(const uint8_t led_index)
{
    leds[led_index].blinking = NOT_BLINKING;
    ledOn(led_index);
}

uint8_t isBlinking(const uint8_t led_index)
{
    return leds[led_index].blinking;
}

ISR(TIMER3_OVF_vect)
{
    int i;
    TCNT3 = 0x3CB0; //timer to (65536 - 50000) = 15536

    for(i = 0; i < LED_AMOUNT; ++i)
    {
        if(isBlinking(i)) {
            ledToggle(i);
        }
    }
}
