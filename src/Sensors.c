#include <avr/interrupt.h>
#include <stdint.h>

#include "Arduino.h"
#include "Sensors.h"

static uint8_t currentChannel;
static uint16_t batteryLevels[3];

void initSensors()
{
    //Timer4 Setup
    //Conversion every 1 ms
    TCCR1A  = 0x00;
    TCCR1B  = 0x02; //prescaler 8
    TCCR1C  = 0x00;
    OCR1BH  = 0x4E;
    OCR1BL  = 0x20;
}

void updateSensors()
{

}

angles getAngles()
{
    
}

ISR(TIMER4_OVF_vect)
{
    int i;
    TCNT4 = 0x3CB0; //timer to (65536 - 50000) = 15536

    for(i = 0; i < LED_AMOUNT; ++i)
    {
        if(isBlinking(i)) {
            ledToggle(i);
        }
    }
}
