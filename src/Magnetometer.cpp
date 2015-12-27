#include "Magnetometer.h"

#define ADDRESS_MAG                     (0x3C >> 1)         // 0011110x

#define REGISTER_MAG_CRA_REG_M          0x00
#define REGISTER_MAG_CRB_REG_M          0x01
#define REGISTER_MAG_MR_REG_M           0x02
#define REGISTER_MAG_OUT_X_H_M          0x03

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
    // Set gain +- 2.5 Gauss
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

    measure(0.0, 0.0);
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
    uint8_t zhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t ylo = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    uncalibrated[0] = (int16_t)(xlo | ((int16_t)xhi << 8));
    uncalibrated[1] = (int16_t)(ylo | ((int16_t)yhi << 8));
    uncalibrated[2] = (int16_t)(zlo | ((int16_t)zhi << 8));

    calibrate();
}

void Magnetometer::calibrate()
{
    if (uncalibrated[0] == 0.0 &&
        uncalibrated[1] == 0.0 &&
        uncalibrated[2] == 0.0) {
        return;
    }

    double result[3] = { 0.0 , 0.0 , 0.0 };

    uncalibrated[0] /= 6.7;
    uncalibrated[1] /= 6.7;
    uncalibrated[2] /= 6.0;

    for (uint8_t i = 0; i < 3; ++i) {
        uncalibrated[i] -= B[i];
    }

    for (uint8_t i = 0; i < 3; ++i) {
        for (uint8_t j = 0; j < 3; ++j) {
            result[i] += T[i][j] * uncalibrated[j];
        }
    }

    magData.x = result[0];
    magData.y = result[1];
    magData.z = result[2];
}

void Magnetometer::measure(float aRoll, float aPitch)
{
    read();    

    float roll = aPitch / 180.0f * PI;
    float pitch = -aRoll / 180.0f * PI;
    float sinRoll = sin(roll);
    float cosRoll = cos(roll);
    float sinPitch  = sin(pitch);
    float cosPitch  = cos(pitch);

    float magX = magData.x * cosPitch +
                 magData.y * sinRoll * sinPitch +
                 magData.z * cosRoll * sinPitch;
    float magY = magData.z * sinRoll - magData.y * cosRoll;

    float tmp = sqrt(magX * magX + magY * magY);
    
    hdgX = magX / tmp;
    hdgY = magY / tmp;
}

float Magnetometer::getAbsoluteHeading()
{
    float absoluteHeading = atan2(hdgY, hdgX);

    if (absoluteHeading < 0) {
        absoluteHeading += (2 * PI);
    }

    absoluteHeading -= (PI / 2);

    return absoluteHeading;
}
