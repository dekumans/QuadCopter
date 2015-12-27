#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include "Arduino.h"
#include "Pilot.h"
#include "AHRS.h"
#include "Motors.h"
#include "PID.h"

class FlightController
{
    public:
        FlightController(Pilot& pilot);
        void init();
        void process(float timeStep);
        void zeroIntegralError();

        Pilot& pilot;
        AHRS ahrs;
        Motors motors;

        PID ratePitchPID;
        PID rateRollPID;
        PID rateYawPID;
        PID attitudePitchPID;
        PID attitudeRollPID;
        PID headingHoldPID;

    private:
        void limitMotorCommands();
        float processHeading(float timeStep);

        float setHeading;
        float headingHold;
        bool headingHoldState;
        float headingTime;
};

#endif /* FlightController.h */
