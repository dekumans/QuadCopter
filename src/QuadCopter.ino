#include "QuadCopter.h"

void setup()
{
    Serial.begin(115200);

    ahrs.init();
    batteryMonitor.init();
    remote.init();
    motors.init();
    led.init();

    Serial.println("QuadCopter initialized");
}


void process100HzTask()
{
    timeStep = (currentTime - hundredHzPreviousTime) / 1000000.0f;
    hundredHzPreviousTime = currentTime;

    ahrs.updateAngles(timeStep);

    motors.commandAll(remote.getValue(0));
}

void loop()
{
    currentTime = micros();
    deltaTime = currentTime - previousTime;

    ahrs.readSensorData();

    if(deltaTime > 10000) {
        frameCounter++;

        process100HzTask();

        if((frameCounter % 10) == 0) {
            batteryMonitor.update();
            batteryMonitor.print();

            ahrs.printAngles();

            remote.print();
        }

        if((frameCounter % 25) == 0) {
            led.toggle();
        }

        previousTime = currentTime;
    }

    if(frameCounter >= 100) {
        frameCounter = 0;
    }
}
