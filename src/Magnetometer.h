#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <Wire.h>
#include "Arduino.h"

typedef struct MagData_s
{
    double x;
    double y;
    double z;
} MagData;

const double T[3][3] = {
    { 1.057  , 0.004 , -0.031 },
    { -0.033 , 1.063 , -0.149 },
    { 0.048  , 0.148 , 1.13   }
};

const double B[3] = {
    -5.337 ,
    9.592  ,
    12.651
};

class Magnetometer
{
    public:
        MagData magData;        // Last read magnetometer data will be available here
        float hdgX;
        float hdgY;

        uint8_t init(void);
        void measure(float aRoll, float aPitch);
        float getAbsoluteHeading();

    private:
        double uncalibrated[3];

        void write8(uint8_t address, uint8_t reg, uint8_t value);
        uint8_t read8(uint8_t address, uint8_t reg);
        void read(void);

        void calibrate(void);
};

#endif /* Magnetometer.h */
