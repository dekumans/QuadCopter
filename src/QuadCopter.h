#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include "Arduino.h"
#include "Led.h"
#include "BatteryMonitor.h"
#include "Remote.h"
#include "AHRS.h"
#include "Motors.h"

#define RED_LED_CHANNEL 30

AHRS ahrs;
BatteryMonitor batteryMonitor;
Remote remote;
Motors motors;
Led led(RED_LED_CHANNEL);

float timeStep;

uint32_t currentTime;
uint32_t previousTime;
uint32_t deltaTime;

uint32_t hundredHzPreviousTime;

uint8_t frameCounter;

ISR(PCINT2_vect)
{
    remote.interrupt();
}

#endif /* QuadCopter.h */
