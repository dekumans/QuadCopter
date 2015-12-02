#include "Arduino.h"

#include "AHRS.h"

void AHRS::init()
{
    if(!accel.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Accel) detected ... Check your wiring!"));
        while(1);
    }

    if(!mag.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Mag) detected ... Check your wiring!"));
        while(1);
    }

    if(!gyro.init())
    {
        /* There was a problem detecting the L3DH20 ... check your connections */
        Serial.println(F("Ooops, no L3D20 (Gyro) detected ... Check your wiring!"));
        while(1);
    }

    accel.calibrate();
    mag.calibrate();
    gyro.calibrate();
}

void AHRS::updateAngles()
{
    angles.roll = 0;
    angles.pitch = 0;
    angles.yaw = 0;
}

void AHRS::readSensorData()
{
    accel.read();
    mag.read();
    gyro.read();
}

void AHRS::printSensorData(int a, int m, int g)
{
    if (a != 0) {
        Serial.print("Ax: ");
        Serial.print(accel.x.getMean(), DEC);
        Serial.print(" Ay: ");
        Serial.print(accel.y.getMean(), DEC);
        Serial.print(" Az: ");
        Serial.println(accel.z.getMean(), DEC);
    }

    if (m != 0) {
        Serial.print("Mx: ");
        Serial.print(mag.x.getMean(), DEC);
        Serial.print(" My: ");
        Serial.print(mag.y.getMean(), DEC);
        Serial.print(" Mz: ");
        Serial.println(mag.z.getMean(), DEC);
    }

    if (g != 0) {
        Serial.print("Gx: ");
        Serial.print(x, 2);
        Serial.print(" Gy: ");
        Serial.print(y, 2);
        Serial.print(" Gz: ");
        Serial.println(z, 2);
    }
}
