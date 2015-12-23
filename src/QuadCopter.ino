#include "QuadCopter.h"

void setup()
{
    Serial.begin(115200);

    remote.init();
    pilot.init();
    fc.init();
    telemetry.init();
    led.init();
    batteryMonitor.init();

    Serial.println("QuadCopter initialized");
}

void process100HzTask()
{
    timeStep = (currentTime - hundredHzPreviousTime) / 1000000.0f;
    hundredHzPreviousTime = currentTime;

    fc.process(timeStep);
}

void process50HzTask()
{
    timeStep = (currentTime - fiftyHzPreviousTime) / 1000000.0f;
    fiftyHzPreviousTime = currentTime;

    pilot.update();
}

void loop()
{
    currentTime = micros();
    deltaTime = currentTime - previousTime;

    fc.ahrs.readSensorData();

    if(deltaTime > 10000) {
        frameCounter++;

        process100HzTask();

        if((frameCounter % 20) == 0) {
            process50HzTask();
        }

        if((frameCounter % 10) == 0) {
            // TODO:: calculate heading
            telemetry.update();
        }

        if((frameCounter % 50) == 0) {
            led.toggle();
        }

        previousTime = currentTime;
    }

    if(frameCounter >= 100) {
        frameCounter = 0;
    }
}
