#ifndef AHRS_H
#define AHRS_H

#include "Arduino.h"
#include "Accelerometer.h"
#include "Magnetometer.h"
#include "Gyroscope.h"

#define G_PITCH   0
#define G_ROLL    1
#define G_YAW     2
#define A_PITCH   3
#define A_ROLL    4
#define A_YAW     5

struct Angles {
    float roll;
    float pitch;
    float yaw;
};

class AHRS {
    public:
        void init();
        void updateAngles(float timeStep);
        void readSensorData();
        void printAngles();
        void printSensorData(int a, int m, int g);

        Gyroscope gyro;
        Angles angles;

    private:
        Accelerometer accel;
        Magnetometer mag;

        void argUpdate(float gx, float gy, float gz,
                       float ax, float ay, float az, float timeStep);

        bool isSwitched(float previousError, float currentError);

        float Kp = 0.0;
        float Ki = 0.0;
        float halfT = 0.0;
        float q0 = 0.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;
        float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;

        float previousEx = 0.0;
        float previousEy = 0.0;
        float previousEz = 0.0;
};

#endif /* AHRS.h */
