#include <avr/interrupt.h>
#include <stdint.h>

#include "Arduino.h"
#include "BatteryMonitor.h"

static uint8_t currentChannel;
static uint16_t batteryLevels[3];

void initBatteryMonitor()
{
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);

    //ADC Setup
    ADMUX   = 0x40;
    ADCSRA  = 0xAF; //AD + interrupt enable, prescaler = fclk/128
    ADCSRB  = 0x05; //timer1 compare match B
    DIDR0   = 0x07; //disable digital input ch0 ch1 ch2
    DIDR1   = 0x00; //no digital input disable

    //Timer1 Setup
    //Conversion every 10 ms
    TCCR1A  = 0x00;
    TCCR1B  = 0x02; //prescaler 8
    TCCR1C  = 0x00;
    OCR1BH  = 0x4E;
    OCR1BL  = 0x20;
}

uint8_t checkBattery()
{
    if(batteryLevels[0] > 750) {
        return 1;
    }
    else {
        return 0;
    }
}

uint16_t *getBatteryLevels()
{
    return batteryLevels;
}

void updateBatteryLevels()
{
    uint16_t value = ADC;
    batteryLevels[currentChannel] = value;
    currentChannel++;
    if(currentChannel > 2) {
        currentChannel = 0;
    }
    ADMUX = currentChannel | (0x40);
}

void ADC_ISR() {
    TIFR1 |= _BV(OCF1B); //clear OCF1B interrupt flag
    TCNT1 = 0;
    updateBatteryLevels();
}

ISR(ADC_vect)
{
    ADC_ISR();
}

