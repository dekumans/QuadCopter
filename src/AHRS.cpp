#include "Arduino.h"

extern "C" {
#include "MadgwickAHRS.h"
}

#define timeStep        0.01f
#define giroVar         0.1f
#define deltaGiroVar    0.1f
#define accelVar        5.0f

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
    //mag.calibrate();
    gyro.calibrate();
}

void AHRS::updateAngles()
{
    //called every 10 ms
    
    float aX = accel.x.getMean();
    float aY = accel.y.getMean();
    float aZ = accel.z.getMean();

    float gX = gyro.x.getMean();
    float gY = gyro.y.getMean();
    
    accel_angles.pitch = atan2(aY, aZ) * (180.0f / PI);
    accel_angles.roll = atan2(aX, aZ) * (180.0f / PI);
    accel_angles.yaw = 0.0f;

    gyro_angles.pitch = gyro_angles.pitch + (gX * 0.07f * timeStep);
    gyro_angles.roll = gyro_angles.roll - (gY * 0.07f * timeStep);
    gyro_angles.yaw = 0.0f;

    kalman_angles.pitch = kalman_angles.pitch + (gX * 0.07f * timeStep);
    kalman_angles.roll = kalman_angles.roll - (gY * 0.07f * timeStep);
    kalman_angles.yaw = 0.0f;

    Pxx += timeStep * (2 * Pxv + timeStep * Pvv);
    Pxv += timeStep * Pvv;
    Pxx += timeStep * giroVar;
    Pvv += timeStep * deltaGiroVar;
    kx = Pxx * (1 / (Pxx + accelVar));
    kv = Pxv * (1 / (Pxx + accelVar));

    kalman_angles.roll += (accel_angles.roll - kalman_angles.roll) * kx;
    kalman_angles.pitch += (accel_angles.pitch - kalman_angles.pitch) * kx;

    Pxx *= (1 - kx);
    Pxv *= (1 - kx);
    Pvv -= kv * Pxv;
}

void AHRS::readSensorData()
{
    accel.read();
    mag.read();
    gyro.read();

    //MadgwickAHRSupdateIMU(gyro.x.getMean(), gyro.y.getMean(), gyro.z.getMean(),
    //                   accel.x.getMean(), accel.y.getMean(), accel.z.getMean());
}

void AHRS::printAngles()
{
    Serial.print("Pitch: ");
    Serial.print(kalman_angles.pitch, 3);
    Serial.print(", Roll: ");
    Serial.println(kalman_angles.roll, 3);

    //Serial.print(" Yaw: ");
    //Serial.println(angles.yaw, 3);
}

void AHRS::printSensorData(int a, int m, int g)
{
    if (a != 0) {
        Serial.print("Ax: ");
        Serial.print(accel.x.getMean(), 2);
        Serial.print(" Ay: ");
        Serial.print(accel.y.getMean(), 2);
        Serial.print(" Az: ");
        Serial.println(accel.z.getMean(), 2);
    }

    if (m != 0) {
        Serial.print("Mx: ");
        Serial.print(mag.x.getMean(), 2);
        Serial.print(" My: ");
        Serial.print(mag.y.getMean(), 2);
        Serial.print(" Mz: ");
        Serial.println(mag.z.getMean(), 2);
    }

    if (g != 0) {
        Serial.print("Gx: ");
        Serial.print(gyro.x.getMean(), 2);
        Serial.print(" Gy: ");
        Serial.print(gyro.y.getMean(), 2);
        Serial.print(" Gz: ");
        Serial.println(gyro.z.getMean(), 2);
    }
}
