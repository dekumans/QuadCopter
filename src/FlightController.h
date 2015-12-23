#ifndef FLIGHTCONTROLLER_H
#define FLIGHTCONTROLLER_H

#include "Arduino.h"
#include "AHRS.h"
#include "Motors.h"
#include "Pilot.h"
#include "PID.h"

class FlightController
{
    public:
        FlightController(Pilot& pilot);
        void init();
        void process(float timeStep);

        Pilot& pilot;
        AHRS ahrs;
        Motors motors;

        PID ratePitchPID;
        PID rateRollPID;
        PID rateYawPID;
        PID attitudePitchPID;
        PID attitudeRollPID;

    private:
};

#endif /* FlightController.h */
