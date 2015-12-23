#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <Wire.h>

#include "Arduino.h"
#include "Filter.h"

typedef struct GyroData_s
{
    int16_t x;
    int16_t y;
    int16_t z;
} GyroData;

class Gyroscope
{
    public:
        Filter x; //Filtered gyroscope data x-axis
        Filter y; //Filtered gyroscope data y-axis
        Filter z; //Filtered gyroscope data z-axis

        GyroData gyroData;        // Last read gyroscope data will be available here
        GyroData calibrateData;

        void read(void);
        void calibrate(void);
        uint8_t init(void);

    private:
        void write8(uint8_t reg, uint8_t value);
        uint8_t read8(uint8_t reg);
};

#endif /* Gyroscope.h */
