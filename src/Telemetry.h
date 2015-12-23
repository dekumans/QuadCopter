#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "FlightController.h"
#include "Led.h"

class Telemetry
{
    public:
        Telemetry(FlightController& fc);
        void init();
        void update();

    private:
        FlightController& fc;

        float P;
        float I;
        float D;

        float receiveFloat();
        void receivePIDfactors();
};

#endif /* Telemetry.h */
