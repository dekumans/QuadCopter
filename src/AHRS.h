#include "Arduino.h"

#include "Accelerometer.h"
#include "Magnetometer.h"
#include "Gyroscope.h"

struct Angles {
    float roll;
    float pitch;
    float yaw;
};

class AHRS {
    public:
        void init();
        void updateAngles();
        void readSensorData();
        void printSensorData(int a, int m, int g);

        Angles angles;

    private:
        Accelerometer accel;
        Magnetometer mag;
        Gyroscope gyro;
};
