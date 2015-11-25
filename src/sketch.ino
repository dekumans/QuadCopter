#include "Arduino.h"
#include "Led.h"
#include "BatteryMonitor.h"
#include "Accelerometer.h"
#include "Magnetometer.h"

Accelerometer accel;
Magnetometer mag;

void initSensors()
{
    if(!accel.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Accel) detected ... Check your wiring!"));
        while(1);
    }

    if(!mag.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Mag) detected ... Check your wiring!"));
        while(1);
    }

    accel.calibrate();
    mag.calibrate();
}

void printBatteryLevels() {
    uint16_t *levels = getBatteryLevels();
    Serial.print(levels[0], DEC);
    Serial.print(" ");
    Serial.print(levels[1], DEC);
    Serial.print(" ");
    Serial.println(levels[2], DEC);
}

void printMagData() {
    Serial.print("Mx: ");
    Serial.print(mag.magData.x, DEC);
    Serial.print(" My: ");
    Serial.print(mag.magData.y, DEC);
    Serial.print(" Mz: ");
    Serial.println(mag.magData.z, DEC);
}

void printFilteredData() {
    Serial.print("Fx: ");
    Serial.print(accel.x.getMean(), DEC);
    Serial.print(" Fy: ");
    Serial.print(accel.y.getMean(), DEC);
    Serial.print(" Fz: ");
    Serial.println(accel.z.getMean(), DEC);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello");

    initBatteryMonitor();
    initLeds();
    initSensors();
}

int i = 0;
void loop()
{
    if (i > 100) {
        printMagData();
        i = 0;
    }

    accel.read();
    mag.read();

    i++;
    delay(2);
}

