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
    rateRollPID.init(80.0, 150.0, 0.01);
    rateYawPID.init(100.0, 250.0, 0.1);
    attitudePitchPID.init(8.0, 0.0, 0.0);
    attitudeRollPID.init(8.0, 0.0, 0.0);
    headingHoldPID.init(10.0, 1.0, 0.0);
}

void FlightController::process(float timeStep)
{
    ahrs.updateAngles(timeStep);

    float throttle = pilot.commands.throttle;

    float remotePitch = pilot.commands.pitch * REMOTE_FACTOR;
    float remoteRoll = pilot.commands.roll * REMOTE_FACTOR;

    float attitudePitch = GYRO_FACTOR * attitudePitchPID.update(remotePitch, ahrs.angles.pitch, timeStep);
    float attitudeRoll = GYRO_FACTOR * attitudeRollPID.update(remoteRoll, ahrs.angles.roll, timeStep);
    
    float gyroPitch = ahrs.gyro.x.lastValue;
    float gyroRoll = ahrs.gyro.y.lastValue;

    float commandPitch = ratePitchPID.update(attitudePitch, gyroPitch, timeStep);
    float commandRoll = rateRollPID.update(attitudeRoll, -gyroRoll, timeStep);
    float commandYaw = processHeading(timeStep);

    if (pilot.motorArmed && pilot.safetyCheck) {
        motors.motorCommand[MOTOR_FR] = throttle - commandPitch - commandRoll - commandYaw;
        motors.motorCommand[MOTOR_FL] = throttle - commandPitch + commandRoll + commandYaw;
        motors.motorCommand[MOTOR_RR] = throttle + commandPitch - commandRoll + commandYaw;
        motors.motorCommand[MOTOR_RL] = throttle + commandPitch + commandRoll - commandYaw;
    }

    limitMotorCommands();

    if (pilot.motorArmed && pilot.safetyCheck) {
        motors.command();
    }
}

float FlightController::processHeading(float timeStep)
{
    float heading = ahrs.hdg.trueNorthHeading * 180.0 / PI;
    float relativeHeading = heading - setHeading;
    if (heading <= (setHeading - 180)) {
        relativeHeading += 360;
    }
    if (heading >= (setHeading + 180)) {
        relativeHeading -= 360;
    }

    if (pilot.commands.throttle > MINTHROTTLE) {
        if (pilot.commands.yaw < -25 || pilot.commands.yaw > 25) {
            setHeading = heading;
            headingHold = 0.0;
            headingHoldPID.zeroIntegralError();
            headingHoldState = false;
        }
        else {
            if (relativeHeading > -0.25 && relativeHeading < 0.25) {
                headingHold = 0.0;
                headingHoldPID.zeroIntegralError();
            }
            else if (!headingHoldState) {
                if (headingTime > 500000) {
                    headingHoldState = true;
                    headingTime = 0.0;
                    setHeading = heading;
                    headingHold = 0.0;
                }
                else {
                    headingTime += timeStep;
                }
            }
            else {
                headingHold = headingHoldPID.update(0.0, relativeHeading, timeStep);
            }
        }
    }
    else {
        setHeading = heading;
        headingHold = 0.0;
        headingHoldPID.zeroIntegralError();
    }

    float remoteYaw = pilot.commands.yaw * GYRO_FACTOR;
    remoteYaw = constrain(remoteYaw + (headingHold / 180.0 * PI), -PI, PI);
    float commandYaw = rateYawPID.update(remoteYaw, -ahrs.gyro.z.lastValue, timeStep); 
    //Serial.println(commandYaw, 3);
    return commandYaw;
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
