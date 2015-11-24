#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

//extern "C" {

#include <Wire.h>

#include "Arduino.h"

#define ADDRESS_ACCEL           (0x32 >> 1)         // 0011001x
#define CALIBRATION_STEPS       50

typedef enum
{                                                // DEFAULT    TYPE
    REGISTER_ACCEL_CTRL_REG1_A         = 0x20,   // 00000111   rw
    REGISTER_ACCEL_CTRL_REG2_A         = 0x21,   // 00000000   rw
    REGISTER_ACCEL_CTRL_REG3_A         = 0x22,   // 00000000   rw
    REGISTER_ACCEL_CTRL_REG4_A         = 0x23,   // 00000000   rw
    REGISTER_ACCEL_CTRL_REG5_A         = 0x24,   // 00000000   rw
    REGISTER_ACCEL_CTRL_REG6_A         = 0x25,   // 00000000   rw
    REGISTER_ACCEL_REFERENCE_A         = 0x26,   // 00000000   r
    REGISTER_ACCEL_STATUS_REG_A        = 0x27,   // 00000000   r
    REGISTER_ACCEL_OUT_X_L_A           = 0x28,
    REGISTER_ACCEL_OUT_X_H_A           = 0x29,
    REGISTER_ACCEL_OUT_Y_L_A           = 0x2A,
    REGISTER_ACCEL_OUT_Y_H_A           = 0x2B,
    REGISTER_ACCEL_OUT_Z_L_A           = 0x2C,
    REGISTER_ACCEL_OUT_Z_H_A           = 0x2D,
    REGISTER_ACCEL_FIFO_CTRL_REG_A     = 0x2E,
    REGISTER_ACCEL_FIFO_SRC_REG_A      = 0x2F,
    REGISTER_ACCEL_INT1_CFG_A          = 0x30,
    REGISTER_ACCEL_INT1_SOURCE_A       = 0x31,
    REGISTER_ACCEL_INT1_THS_A          = 0x32,
    REGISTER_ACCEL_INT1_DURATION_A     = 0x33,
    REGISTER_ACCEL_INT2_CFG_A          = 0x34,
    REGISTER_ACCEL_INT2_SOURCE_A       = 0x35,
    REGISTER_ACCEL_INT2_THS_A          = 0x36,
    REGISTER_ACCEL_INT2_DURATION_A     = 0x37,
    REGISTER_ACCEL_CLICK_CFG_A         = 0x38,
    REGISTER_ACCEL_CLICK_SRC_A         = 0x39,
    REGISTER_ACCEL_CLICK_THS_A         = 0x3A,
    REGISTER_ACCEL_TIME_LIMIT_A        = 0x3B,
    REGISTER_ACCEL_TIME_LATENCY_A      = 0x3C,
    REGISTER_ACCEL_TIME_WINDOW_A       = 0x3D
} AccelRegisters;


typedef struct AccelData_s
{
    int16_t x;
    int16_t y;
    int16_t z;
} AccelData;

class Accelerometer
{
    public:
        AccelData accelData;   // Last read accelerometer data will be available here
        AccelData calibrateData;

        void readAccelerometer(void);
        void calibrateAccelerometer(void);
        uint8_t initAccelerometer(void);

    private:
        void write8(uint8_t address, uint8_t reg, uint8_t value);
        uint8_t read8(uint8_t address, uint8_t reg);
};

//}

#endif /* Accelerometer.h */
