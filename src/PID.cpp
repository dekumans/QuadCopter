#include "Arduino.h"
#include "PID.h"

void PID::init(float initP, float initI, float initD)
{
    setPIDfactors(initP, initI, initD);
    windupGuard = 1000.0; 
    rampValue = 50.0;
    integratedError = 0.0;
    lastUpdate = 0.0;
}

float PID::update(float targetPosition, float currentPosition, float timeStep)
{
    float error = targetPosition - currentPosition;
    integratedError += (error * timeStep);
    integratedError = constrain(integratedError, -windupGuard, windupGuard);
    float dTerm = D * (currentPosition - lastError) / timeStep;
    lastError = currentPosition;

    float update = (P * error) + (I * integratedError) + dTerm;

    //prevent to fast changes
    if ((update - lastUpdate) > rampValue) {
        update = rampValue;
    }
    if ((lastUpdate - update) > rampValue) {
        update = -rampValue;
    }
    lastUpdate = update;

    return update;
}

void PID::zeroIntegralError()
{
    integratedError = 0.0;
}

void PID::setPIDfactors(float newP, float newI, float newD)
{
    P = newP;
    I = newI;
    D = newD;
}

void PID::setRampValue(float newValue)
{
    rampValue = newValue;
}

void PID::printPIDfactors()
{
    Serial1.print("PID;");
    Serial1.print(P, 2);
    Serial1.print(I, 2);
    Serial1.println(D, 2);
}
