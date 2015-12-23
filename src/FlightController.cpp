#include "FlightController.h"

#define SMOOTH_FACTOR   0.15f
#define REMOTE_FACTOR   0.1f
#define GYRO_FACTOR     0.001f

FlightController::FlightController(Pilot& pilot) :
    pilot(pilot)
{}

void FlightController::init()
{
    ahrs.init();
    motors.init();

    ratePitchPID.init(95.0, 470.0, 0.01);
    rateRollPID.init(95.0, 470.0, 0.01);
    rateYawPID.init(95.0, 300.0, 0.0);
    attitudePitchPID.init(8.0, 0.0, 0.0);
    attitudeRollPID.init(8.0, 0.0, 0.0);
}

void FlightController::process(float timeStep)
{
    ahrs.updateAngles(timeStep);

    float throttle = pilot.commands.throttle;

    if (throttle == 1000) {
        ratePitchPID.zeroIntegralError();
        rateRollPID.zeroIntegralError();
        rateYawPID.zeroIntegralError();
        attitudePitchPID.zeroIntegralError();
        attitudeRollPID.zeroIntegralError();
    }

    float remotePitch = pilot.commands.pitch;
    float remoteRoll = pilot.commands.roll;
    float remoteYaw = pilot.commands.yaw;

    float attitudePitch = GYRO_FACTOR * attitudePitchPID.update(remotePitch, ahrs.angles.pitch, timeStep);
    float attitudeRoll = GYRO_FACTOR * attitudeRollPID.update(remoteRoll, ahrs.angles.roll, timeStep);
    
    float gyroPitch = ahrs.gyro.x.lastValue;
    float gyroRoll = ahrs.gyro.y.lastValue;
    float gyroYaw = ahrs.gyro.z.lastValue;

    float commandPitch = ratePitchPID.update(attitudePitch, gyroPitch, timeStep);
    float commandRoll = rateRollPID.update(attitudeRoll, -gyroRoll, timeStep);
    float commandYaw = rateYawPID.update(remoteYaw, gyroYaw, timeStep);

    motors.motorCommand[MOTOR_FR] = throttle - commandPitch - commandRoll + commandYaw;
    motors.motorCommand[MOTOR_FL] = throttle - commandPitch + commandRoll - commandYaw;
    motors.motorCommand[MOTOR_RR] = throttle + commandPitch - commandRoll - commandYaw;
    motors.motorCommand[MOTOR_RL] = throttle + commandPitch + commandRoll + commandYaw;

    motors.command();
    //motors.commandAll(remote.getValue(0));
}
