#include "Arduino.h"
#include "Led.h"
#include "BatteryMonitor.h"
#include "Accelerometer.h"

#define FILTER_SIZE 25

Accelerometer accel;

AccelData filteredData;
AccelData filter[FILTER_SIZE];

void initSensors()
{
    if(!accel.initAccelerometer())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Accel) detected ... Check your wiring!"));
        while(1);
    }

    accel.calibrateAccelerometer();
}

void printBatteryLevels() {
    uint16_t *levels = getBatteryLevels();
    Serial.print(levels[0], DEC);
    Serial.print(" ");
    Serial.print(levels[1], DEC);
    Serial.print(" ");
    Serial.println(levels[2], DEC);
}

void printAccelData() {
    Serial.print("Ax: ");
    Serial.print(accel.accelData.x, DEC);
    Serial.print(" Ay: ");
    Serial.print(accel.accelData.y, DEC);
    Serial.print(" Az: ");
    Serial.println(accel.accelData.z, DEC);
}

void printFilteredData() {
    Serial.print("Fx: ");
    Serial.print(filteredData.x, DEC);
    Serial.print(" Fy: ");
    Serial.print(filteredData.y, DEC);
    Serial.print(" Fz: ");
    Serial.println(filteredData.z, DEC);
}

void calculateFilteredData() {
    filteredData.x = 0;
    filteredData.y = 0;
    filteredData.z = 0;

    int j;
    for (j = 0; j < FILTER_SIZE; j++) {
        filteredData.x += filter[j].x;
        filteredData.y += filter[j].y;
        filteredData.z += filter[j].z;
    }

    filteredData.x /= FILTER_SIZE;
    filteredData.y /= FILTER_SIZE;
    filteredData.z /= FILTER_SIZE;
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
    if (i == FILTER_SIZE) {
        calculateFilteredData();
        printFilteredData();
        i = 0;
    }

    accel.readAccelerometer();
    filter[i].x = accel.accelData.x;
    filter[i].y = accel.accelData.y;
    filter[i].z = accel.accelData.z;

    i++;
    delay(4);
}

