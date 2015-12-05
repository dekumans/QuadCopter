#include <Wire.h>

#include "Arduino.h"
#include "Gyroscope.h"
#include "Filter.h"

#define ADDRESS_GYRO                    (0x6B)        // 1101011
#define CALIBRATION_STEPS               40
#define L3GD20_ID                       0xD4
#define L3GD20H_ID                      0xD7

#define GYRO_REGISTER_WHO_AM_I          0x0F
#define GYRO_REGISTER_CTRL_REG1         0x20
#define GYRO_REGISTER_CTRL_REG2         0x21
#define GYRO_REGISTER_CTRL_REG4         0x23
#define GYRO_REGISTER_CTRL_REG5         0x24
#define GYRO_REGISTER_OUT_X_L           0x28

#define GYRO_SENSITIVITY_2000DPS        0.070f              // Roughly 18/256

void Gyroscope::write8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(ADDRESS_GYRO);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t Gyroscope::read8(uint8_t reg)
{
    uint8_t value;

    Wire.beginTransmission(ADDRESS_GYRO);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t)ADDRESS_GYRO, (uint8_t)1);
    while (!Wire.available());
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

uint8_t Gyroscope::init()
{
    // Enable I2C
    Wire.begin();

    uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
    if ((id != L3GD20_ID) && (id != L3GD20H_ID)) {
        return 0;
    }

    // Enable the gyroscope (380Hz, 25 cut-off)
    write8(GYRO_REGISTER_CTRL_REG1, 0x00);
    write8(GYRO_REGISTER_CTRL_REG1, 0x9F);
    // High pass settings
    write8(GYRO_REGISTER_CTRL_REG2, 0x02);
    // +- 2000 dps
    write8(GYRO_REGISTER_CTRL_REG4, 0x20);
    // Enable High Pass
    write8(GYRO_REGISTER_CTRL_REG5, 0x10);

    return 1;
}

void Gyroscope::read()
{
    // Read the accelerometer
    Wire.beginTransmission((uint8_t)ADDRESS_GYRO);
    Wire.write(GYRO_REGISTER_OUT_X_L | 0x80);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)ADDRESS_GYRO, (uint8_t)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    uint8_t xlo = Wire.read();
    uint8_t xhi = Wire.read();
    uint8_t ylo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t zhi = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    gyroData.x = (int16_t)(xlo | (xhi << 8));
    gyroData.y = (int16_t)(ylo | (yhi << 8));
    gyroData.z = (int16_t)(zlo | (zhi << 8));

    // Substract calibration data
    gyroData.x -= calibrateData.x;
    gyroData.y -= calibrateData.y;
    gyroData.z -= calibrateData.z;

    x.addValue(gyroData.x);
    y.addValue(gyroData.y);
    z.addValue(gyroData.z);
}

void Gyroscope::calibrate()
{
    delay(100);

    int i;
    int cX, cY, cZ;

    cX = 0;
    cY = 0;
    cZ = 0;

    for (i = 0; i < CALIBRATION_STEPS; i++) {
        read();
        cX += gyroData.x;
        cY += gyroData.y;
        cZ += gyroData.z;
        delay(25);
    }

    calibrateData.x = cX / CALIBRATION_STEPS;
    calibrateData.y = cY / CALIBRATION_STEPS;
    calibrateData.z = cZ / CALIBRATION_STEPS;
}
