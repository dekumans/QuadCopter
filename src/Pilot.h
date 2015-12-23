#ifndef PILOT_H
#define PILOT_H

#include "Arduino.h"
#include "Remote.h"

#define SMOOTH_FACTOR       0.2f

#define MINCHECK            ((MINCOMMAND + 100.0) - MIDCOMMAND)
#define MAXCHECK            ((MAXCOMMAND - 100.0) - MIDCOMMAND)
#define MINTHROTTLE         (MINCOMMAND + 100.0)
#define MAXTHROTTLE         (MAXCOMMAND)
#define MINARMEDTHROTTLE    1150.0

typedef struct pilotCommands_t {
    float throttle;
    float pitch;
    float roll;
    float yaw;
} pilotCommands;

class FlightController;

class Pilot
{
    public:
        Pilot(Remote& remote);
        void init();
        void update(FlightController& fc);

        Remote& remote;

        pilotCommands commands;
        bool motorArmed;
        bool inFlight;
        bool safetyCheck;

    private:
        void updateCommands();
        void processZeroThrottle(FlightController& fc);
        void pulseMotors(FlightController& fc, uint8_t nPulses);

        pilotCommands previousCommands;

};

#endif /* Pilot.h */
