#include "Accelerometer.h"

#define ADDRESS_ACCEL                   (0x32 >> 1)         // 0011001x
#define CALIBRATION_STEPS               50

#define REGISTER_ACCEL_CTRL_REG1_A      0x20
#define REGISTER_ACCEL_CTRL_REG4_A      0x23
#define REGISTER_ACCEL_CTRL_REG4_A      0x23
#define REGISTER_ACCEL_OUT_X_L_A        0x28

#define Accel_MG_LSB                    0.002f              // 1, 2, 4 or 12 mg per lsb

void Accelerometer::write8(uint8_t address, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t Accelerometer::read8(uint8_t address, uint8_t reg)
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

uint8_t Accelerometer::init()
{
    // Enable I2C
    Wire.begin();

    // Enable the accelerometer (1.344kHz)
    write8(ADDRESS_ACCEL, REGISTER_ACCEL_CTRL_REG1_A, 0x97);
    // High resolution, +- 4g
    write8(ADDRESS_ACCEL, REGISTER_ACCEL_CTRL_REG4_A, 0x18);

    // LSM303DLHC has no WHOAMI register so read CTRL_REG1_A back to check
    // if we are connected or not
    uint8_t reg1_a = read8(ADDRESS_ACCEL, REGISTER_ACCEL_CTRL_REG1_A);
    if (reg1_a != 0x97) {
        return 0;
    }
    return 1;

    x.init(0.0, 0.0);
    y.init(0.0, 0.0);
    z.init(-500.0, -500.0);
}

void Accelerometer::read()
{
    // Read the accelerometer
    Wire.beginTransmission((uint8_t)ADDRESS_ACCEL);
    Wire.write(REGISTER_ACCEL_OUT_X_L_A | 0x80);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)ADDRESS_ACCEL, (uint8_t)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    uint8_t xlo = Wire.read();
    uint8_t xhi = Wire.read();
    uint8_t ylo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t zhi = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    accelData.x = (int16_t)(xlo | (xhi << 8)) >> 4;
    accelData.y = (int16_t)(ylo | (yhi << 8)) >> 4;
    accelData.z = (int16_t)(zlo | (zhi << 8)) >> 4;

    // Substract calibration data
    accelData.x -= calibrateData.x;
    accelData.y -= calibrateData.y;
    accelData.z -= calibrateData.z;

    x.addValue(accelData.x);
    y.addValue(accelData.y);
    z.addValue(accelData.z);
}

void Accelerometer::calibrate()
{
    calibrateData.x = 0;
    calibrateData.y = 0;
    calibrateData.z = 0;

    delay(100);

    int i;
    int cX, cY, cZ;

    cX = 0;
    cY = 0;
    cZ = 0;

    for (i = 0; i < CALIBRATION_STEPS; i++) {
        read();
        cX += accelData.x;
        cY += accelData.y;
        cZ += accelData.z;
        delay(5);
    }

    calibrateData.x = cX / CALIBRATION_STEPS;
    calibrateData.y = cY / CALIBRATION_STEPS;
    calibrateData.z = (cZ / CALIBRATION_STEPS) - 500;
}
