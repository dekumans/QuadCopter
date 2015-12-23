#ifndef PILOT_H
#define PILOT_H

#include "Arduino.h"
#include "Remote.h"

#define SMOOTH_FACTOR   0.15f
#define REMOTE_FACTOR   0.1f
#define GYRO_FACTOR     0.001f

typedef struct pilotCommands_t {
    float throttle;
    float pitch;
    float roll;
    float yaw;
} pilotCommands;

class Pilot
{
    public:
        Pilot(Remote& remote);
        void init();
        void update();

        Remote& remote;
        pilotCommands commands;

    private:
        pilotCommands previousCommands;

};

#endif /* Pilot.h */
