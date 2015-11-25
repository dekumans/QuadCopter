#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include "Arduino.h"
#include "Filter.h"

typedef struct MagData_s
{
    int16_t x;
    int16_t y;
    int16_t z;
} MagData;

class Magnetometer
{
    public:
        Filter x; //Filtered magnetometer data x-axis
        Filter y; //Filtered magnetometer data y-axis
        Filter z; //Filtered magnetometer data z-axis

        MagData magData;        // Last read magnetometer data will be available here
        MagData calibrateData;

        void read(void);
        void calibrate(void);
        uint8_t init(void);

    private:
        void write8(uint8_t address, uint8_t reg, uint8_t value);
        uint8_t read8(uint8_t address, uint8_t reg);
};

#endif /* Magnetometer.h */
