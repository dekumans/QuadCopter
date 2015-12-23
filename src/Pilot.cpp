#include "Pilot.h"

Pilot::Pilot(Remote& remote) :
    remote(remote)
{}

void Pilot::init()
{
    remote.init();

    commands.throttle = 1000.0;
    commands.pitch = 0.0;
    commands.roll = 0.0;
    commands.yaw = 0.0;

    previousCommands.throttle = 1000.0;
    previousCommands.pitch = 0.0;
    previousCommands.roll = 0.0;
    previousCommands.yaw = 0.0;
}

void Pilot::update()
{
    commands.throttle = remote.getValue(THROTTLE);
    commands.pitch = remote.getValue(PITCH);
    commands.roll = remote.getValue(ROLL);
    commands.yaw = remote.getValue(YAW);

    commands.pitch = (commands.pitch - 1500.0) * REMOTE_FACTOR;
    commands.roll = (commands.roll - 1500.0) * REMOTE_FACTOR;
    commands.yaw = (commands.yaw - 1500.0) * GYRO_FACTOR;
        
    commands.pitch = (previousCommands.pitch * (1 - SMOOTH_FACTOR)) + (commands.pitch * SMOOTH_FACTOR);
    commands.roll = (previousCommands.roll * (1 - SMOOTH_FACTOR)) + (commands.roll * SMOOTH_FACTOR);
    commands.yaw = (previousCommands.yaw * (1 - SMOOTH_FACTOR)) + (commands.yaw * SMOOTH_FACTOR);

    previousCommands.pitch = commands.pitch;
    previousCommands.roll = commands.roll;
    previousCommands.yaw = commands.yaw;
}
