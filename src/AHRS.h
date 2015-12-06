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
        void updateAngles(float timeStep);
        void readSensorData();
        void printAngles();
        void printSensorData(int a, int m, int g);

        Angles accel_angles;
        Angles gyro_angles;
        Angles kalman_angles;

    private:
        Accelerometer accel;
        Magnetometer mag;
        Gyroscope gyro;

        float Pxx = 0.1;
        float Pvv = 0.1;
        float Pxv = 0.1;
        float kx;
        float kv;
};
