#include "Arduino.h"
#include "Telemetry.h"

Telemetry::Telemetry(FlightController& fc) :
    fc(fc)
{}

void Telemetry::init()
{
    Serial1.begin(9600);
}

void Telemetry::update()
{
    char c = 0;
    float receiveValue;

    fc.ahrs.printAngles();
    fc.motors.printMotorCommands();
    fc.pilot.remote.print();

    if (Serial1.available() > 0) {
        c = Serial1.read();
    }

    switch (c) {
        case 'A':
            receivePIDfactors();
            fc.ratePitchPID.setPIDfactors(P, I, D);
            fc.ratePitchPID.printPIDfactors();
            break;
        case 'B':
            receivePIDfactors();
            fc.rateRollPID.setPIDfactors(P, I, D);
            fc.rateRollPID.printPIDfactors();
            break;
        case 'C':
            receivePIDfactors();
            fc.attitudePitchPID.setPIDfactors(P, I, D);
            fc.attitudePitchPID.printPIDfactors();
            break;
        case 'D':
            receiveValue = receiveFloat();
            fc.attitudePitchPID.setRampValue(receiveValue);
            fc.attitudeRollPID.setRampValue(receiveValue);
            fc.ratePitchPID.setRampValue(receiveValue);
            fc.rateRollPID.setRampValue(receiveValue);
            break;
        default:
            break;
    }
}

float Telemetry::receiveFloat()
{
    String factor = "";
    char c;
    while (Serial1.available() > 0) {
        c = Serial1.read();
        if (c == ';') {
            return factor.toFloat();
        }
        else {
            factor += c;
        }
    }
    return 0.0;
}

void Telemetry::receivePIDfactors()
{
    P = receiveFloat();
    I = receiveFloat();
    D = receiveFloat();
}
