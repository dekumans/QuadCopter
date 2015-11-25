#include <Wire.h>

#include "Arduino.h"
#include "Magnetometer.h"
#include "Filter.h"

#define ADDRESS_MAG                     (0x3C >> 1)         // 0011110x
#define CALIBRATION_STEPS               50

#define REGISTER_MAG_CRA_REG_M          0x00
#define REGISTER_MAG_CRB_REG_M          0x01
#define REGISTER_MAG_MR_REG_M           0x02
#define REGISTER_MAG_OUT_X_H_M          0x03

#define Mag_Gauss_LSB_XY                450.0F             // Varies with gain
#define Mag_Gauss_LSB_Z                 400.0F              // Varies with gain

void Magnetometer::write8(uint8_t address, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t Magnetometer::read8(uint8_t address, uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (uint8_t)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

uint8_t Magnetometer::init()
{
    // Enable I2C
    Wire.begin();

    // 220 Hz datarate
    write8(ADDRESS_MAG, REGISTER_MAG_CRA_REG_M, 0x1C);
    // Set gain +- 4 Gauss
    write8(ADDRESS_MAG, REGISTER_MAG_CRB_REG_M, 0x60);
    // Enable the magnetometer
    write8(ADDRESS_MAG, REGISTER_MAG_MR_REG_M, 0x00);

    // LSM303DLHC has no WHOAMI register so read CRA_REG_M to check
    // the default value (0b00010000/0x10)
    uint8_t reg1_a = read8(ADDRESS_MAG, REGISTER_MAG_CRA_REG_M);
    if (reg1_a != 0x1C)
    {
        return 0;
    }
    return 1;
}

void Magnetometer::read()
{
    // Read the accelerometer
    Wire.beginTransmission((uint8_t)ADDRESS_MAG);
    Wire.write(REGISTER_MAG_OUT_X_H_M);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)ADDRESS_MAG, (uint8_t)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    uint8_t xhi = Wire.read();
    uint8_t xlo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t ylo = Wire.read();
    uint8_t zhi = Wire.read();
    uint8_t zlo = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
    magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
    magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));

    /*
    // Substract calibration data
    magData.x -= calibrateData.x;
    magData.y -= calibrateData.y;
    magData.z -= calibrateData.z;
    */

    x.addValue(magData.x);
    y.addValue(magData.y);
    z.addValue(magData.z);
}

void Magnetometer::calibrate()
{
    delay(100);

    int i;
    int cX, cY, cZ;

    cX = 0;
    cY = 0;
    cZ = 0;

    for (i = 0; i < CALIBRATION_STEPS; i++) {
        read();
        cX += magData.x;
        cY += magData.y;
        cZ += magData.z;
        delay(10);
    }

    calibrateData.x = cX / CALIBRATION_STEPS;
    calibrateData.y = cY / CALIBRATION_STEPS;
    calibrateData.z = (cZ / CALIBRATION_STEPS) - 500;
}
