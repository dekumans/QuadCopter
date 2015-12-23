#include "FlightController.h"

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
    rateYawPID.init(10.0, 0.0, 0.0);
    attitudePitchPID.init(8.0, 0.0, 0.0);
    attitudeRollPID.init(8.0, 0.0, 0.0);
}

void FlightController::process(float timeStep)
{
    ahrs.updateAngles(timeStep);

    float throttle = pilot.commands.throttle;

    float remotePitch = pilot.commands.pitch * REMOTE_FACTOR;
    float remoteRoll = pilot.commands.roll * REMOTE_FACTOR;
    float remoteYaw = pilot.commands.yaw * GYRO_FACTOR;

    float attitudePitch = GYRO_FACTOR * attitudePitchPID.update(remotePitch, ahrs.angles.pitch, timeStep);
    float attitudeRoll = GYRO_FACTOR * attitudeRollPID.update(remoteRoll, ahrs.angles.roll, timeStep);
    
    float gyroPitch = ahrs.gyro.x.lastValue;
    float gyroRoll = ahrs.gyro.y.lastValue;
    float gyroYaw = ahrs.gyro.z.lastValue;

    float commandPitch = ratePitchPID.update(attitudePitch, gyroPitch, timeStep);
    float commandRoll = rateRollPID.update(attitudeRoll, -gyroRoll, timeStep);
    float commandYaw = rateYawPID.update(remoteYaw, gyroYaw, timeStep);

    if (pilot.motorArmed && pilot.safetyCheck) {
        motors.motorCommand[MOTOR_FR] = throttle - commandPitch - commandRoll + commandYaw;
        motors.motorCommand[MOTOR_FL] = throttle - commandPitch + commandRoll - commandYaw;
        motors.motorCommand[MOTOR_RR] = throttle + commandPitch - commandRoll - commandYaw;
        motors.motorCommand[MOTOR_RL] = throttle + commandPitch + commandRoll + commandYaw;
    }

    limitMotorCommands();

    if (pilot.motorArmed && pilot.safetyCheck) {
        motors.command();
    }
}

void FlightController::limitMotorCommands()
{
    uint16_t max = motors.motorCommand[MOTOR_FR];

    for (uint8_t i = MOTOR_FL; i <= MOTOR_RL; ++i) {
        if (motors.motorCommand[i] > max) {
            max = motors.motorCommand[i];
        }
    } 

    if (max > MAXTHROTTLE) {
        for (uint8_t i = MOTOR_FR; i <= MOTOR_RL; ++i) {
            motors.motorCommand[i] = motors.motorCommand[i] - (max - MAXTHROTTLE);
        } 
    }

    for (uint8_t i = MOTOR_FR; i <= MOTOR_RL; ++i) {
        motors.motorCommand[i] = constrain(motors.motorCommand[i], MINARMEDTHROTTLE, MAXTHROTTLE);
    } 
}

void FlightController::zeroIntegralError()
{
    ratePitchPID.zeroIntegralError();
    rateRollPID.zeroIntegralError();
    rateYawPID.zeroIntegralError();
    attitudePitchPID.zeroIntegralError();
    attitudeRollPID.zeroIntegralError();
}
