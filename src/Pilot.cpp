#include "Pilot.h"
#include "FlightController.h"

Pilot::Pilot(Remote& remote) :
    remote(remote)
{}

void Pilot::init()
{
    commands.throttle = 1000.0;
    commands.pitch = 0.0;
    commands.roll = 0.0;
    commands.yaw = 0.0;

    previousCommands.throttle = 1000.0;
    previousCommands.pitch = 0.0;
    previousCommands.roll = 0.0;
    previousCommands.yaw = 0.0;

    motorArmed = false;
    inFlight = false;
    safetyCheck = false;
}

void Pilot::update(FlightController& fc)
{
    updateCommands();

    if (commands.throttle < MINTHROTTLE) {
        processZeroThrottle(fc);
    }

    if (!inFlight) {
        if (motorArmed && commands.throttle > MINARMEDTHROTTLE) {
            inFlight = true;
        }
    }
}

void Pilot::updateCommands()
{
    commands.throttle = remote.getValue(THROTTLE);
    commands.pitch = remote.getValue(PITCH);
    commands.roll = remote.getValue(ROLL);
    commands.yaw = remote.getValue(YAW);

    commands.pitch -= MIDCOMMAND;
    commands.roll -= MIDCOMMAND;
    commands.yaw -= MIDCOMMAND;
        
    commands.pitch = (previousCommands.pitch * (1 - SMOOTH_FACTOR)) + (commands.pitch * SMOOTH_FACTOR);
    commands.roll = (previousCommands.roll * (1 - SMOOTH_FACTOR)) + (commands.roll * SMOOTH_FACTOR);
    commands.yaw = (previousCommands.yaw * (1 - SMOOTH_FACTOR)) + (commands.yaw * SMOOTH_FACTOR);

    previousCommands.pitch = commands.pitch;
    previousCommands.roll = commands.roll;
    previousCommands.yaw = commands.yaw;
}

void Pilot::processZeroThrottle(FlightController& fc)
{
    if (commands.yaw < MINCHECK && motorArmed) {
        fc.motors.commandAll(MINCOMMAND);
        motorArmed = false;
        inFlight = false;
    }

    if ((commands.yaw < MINCHECK) && (commands.roll > MAXCHECK) && (commands.pitch < MINCHECK)) {
        fc.ahrs.calibrate();
        fc.zeroIntegralError(); 
        //pulseMotors(fc, 3);
    }

    if ((commands.yaw > MAXCHECK) && !motorArmed && safetyCheck) {
        fc.motors.commandAll(MINTHROTTLE);
        motorArmed = true;
        fc.zeroIntegralError();
    }

    if (commands.yaw > MINCHECK) {
        Serial.println(commands.yaw, 0);
        safetyCheck = true;
    }
}

void Pilot::pulseMotors(FlightController& fc, uint8_t nPulses)
{
    for (uint8_t i = 0; i < nPulses; ++i) {
        fc.motors.commandAll(MINCOMMAND + 100);
        delay(250);
        fc.motors.commandAll(MINCOMMAND);
        delay(250);
    }
}
