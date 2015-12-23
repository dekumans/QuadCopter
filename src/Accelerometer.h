#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Wire.h>

#include "Arduino.h"
#include "Filter.h"

typedef struct AccelData_s
{
    int16_t x;
    int16_t y;
    int16_t z;
} AccelData;

class Accelerometer
{
    public:
        Filter x; //Filtered accelerometer data x-axis
        Filter y; //Filtered accelerometer data y-axis
        Filter z; //Filtered accelerometer data z-axis

        AccelData accelData;        // Last read accelerometer data will be available here
        AccelData calibrateData;

        void read(void);
        void calibrate(void);
        uint8_t init(void);

    private:
        void write8(uint8_t address, uint8_t reg, uint8_t value);
        uint8_t read8(uint8_t address, uint8_t reg);
};

#endif /* Accelerometer.h */
