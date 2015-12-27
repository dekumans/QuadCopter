#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include "Arduino.h"
#include "Remote.h"
#include "Pilot.h"
#include "FlightController.h"
#include "Telemetry.h"
#include "Led.h"
#include "BatteryMonitor.h"

#define RED_LED_CHANNEL 13

Remote remote;
Pilot pilot(remote);
FlightController fc(pilot);
Telemetry telemetry(fc);
Led led(RED_LED_CHANNEL);
BatteryMonitor batteryMonitor;

float timeStep;

uint32_t currentTime;
uint32_t previousTime;
uint32_t deltaTime;

uint32_t hundredHzPreviousTime;
uint32_t fiftyHzPreviousTime;
uint32_t tenHzPreviousTime;
uint32_t lowPriorityTenHzPreviousTime;

uint8_t frameCounter;

ISR(PCINT2_vect)
{
    remote.interrupt();
}

#endif /* QuadCopter.h */
