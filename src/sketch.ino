#include "Arduino.h"

#include "Led.h"
#include "BatteryMonitor.h"
#include "AHRS.h"

AHRS ahrs;
BatteryMonitor batteryMonitor;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello");

    ahrs.init();
    batteryMonitor.init();
    initLeds();

    /*      Timers init       */

    ///Timer4 updates ahrs
    ///interrupt every 1ms
    TCCR4A  = 0x00;
    TCCR4B  = 0x01; //no prescaler
    TCCR4C  = 0x00;
    TIMSK4  = 0x01; //enable timer3 ov interrupt
}

int readSensorDataFlag = 0;
int updateBatteryLevelsFlag = 0;
int printCounter = 0;
int printFlag = 0;

void loop()
{
    if (readSensorDataFlag) {
        readSensorDataFlag = 0;
        ahrs.readSensorData();
    }

    if (updateBatteryLevelsFlag) {
        updateBatteryLevelsFlag = 0;
        batteryMonitor.update();
    }

    if (printFlag) {
        printFlag = 0;
        ahrs.printSensorData(0,0,1);
    }
}

ISR(TIMER4_OVF_vect)
{
    TCNT4 = 0xC180; //timer to (65536 - 16000) = 49536
    readSensorDataFlag = 1;

    printCounter++;
    if (printCounter > 100) {
        printCounter = 0;
        printFlag = 1;
    }
}

ISR(ADC_vect)
{
    TIFR1 |= _BV(OCF1B); //clear OCF1B interrupt flag
    TCNT1 = 0;
    updateBatteryLevelsFlag = 1;
}
