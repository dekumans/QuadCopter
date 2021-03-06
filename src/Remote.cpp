#include "Remote.h"

void Remote::init()
{
    DDRK = 0;
    PORTK = 0; 

    PCICR = 0x04; //enable PCIE2

    PCMSK2 = 0x0F; //enable PCINT23:16

    for(uint8_t i = 0; i < CHANNELS; i++) {
        pinData[i].edge = FALLING_EDGE;
    }

    pinData[THROTTLE].lastGoodWidth = MINCOMMAND;
    pinData[PITCH].lastGoodWidth    = MIDCOMMAND;
    pinData[ROLL].lastGoodWidth     = MIDCOMMAND;
    pinData[YAW].lastGoodWidth      = MIDCOMMAND;
}

void Remote::print()
{
    Serial1.print("Remote;");
    for(int i = 0; i < CHANNELS; ++i) {
        Serial1.print(getValue(i), DEC);
        Serial1.print(";");
    }
    Serial1.println("");
}

uint16_t Remote::getValue(uint8_t channel)
{
    uint8_t oldSREG = SREG;
    cli();
    uint16_t rawValue = pinData[channel].lastGoodWidth;
    SREG = oldSREG;
    
    /*
    if(rawValue < MINCOMMAND) {
        return MINCOMMAND;
    }
    else if(rawValue > MAXCOMMAND) {
        return MAXCOMMAND;
    }
    else {
        return rawValue;
    }
    */
    return rawValue;
}

void Remote::interrupt()
{
    uint8_t bit;
    uint8_t curr;
    uint8_t mask;
    uint8_t pin;
    uint32_t currentTime;
    uint32_t time;

    curr = *portInputRegister(11);
    mask = curr ^ lastState;
    lastState = curr;

    if((mask &= PCMSK2) == 0) {
        return;
    }

    currentTime = micros();

    for(uint8_t i = 0; i < 8; ++i) {
        bit = 0x01 << i;
        if(bit & mask) {
            pin = i;

            if(bit & lastState) {
                time = currentTime - pinData[pin].fallTime;
                pinData[pin].riseTime = currentTime;
                if((time >= MINOFFWIDTH) && (time <= MAXOFFWIDTH)) {
                    pinData[pin].edge = RISING_EDGE;
                }
                else {
                    pinData[pin].edge = FALLING_EDGE; //invalid rising edge
                }
            }
            else {
                time = currentTime - pinData[pin].riseTime;
                pinData[pin].fallTime = currentTime;
                if((time >= MINONWIDTH) && (time <= MAXONWIDTH)
                        && (pinData[pin].edge == RISING_EDGE)) {
                    pinData[pin].lastGoodWidth = time;
                    pinData[pin].edge = FALLING_EDGE;
                }
            }
        }
    }
}
