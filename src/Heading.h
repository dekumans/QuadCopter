#ifndef HEADING_H
#define HEADING_H

#include "Arduino.h"
#include "Magnetometer.h"

struct HeadingAngles {
    float roll;
    float pitch;
    float yaw;
};

class Heading
{
    public:
        void init();
        void measure(float roll, float pitch);
        void calculate(float gX, float gY, float gZ,
                       float aX, float aY, float aZ, float timeStep);
        void printAngles();
        void printMagData();

        HeadingAngles hAngles;
        float trueNorthHeading;

    private:
        Magnetometer mag;

        float lkpAcc = 0.0;                					// proportional gain governs rate of convergence to accelerometer
        float lkiAcc = 0.0;                					// integral gain governs rate of convergence of gyroscope biases
        float lkpMag = 0.0;                					// proportional gain governs rate of convergence to magnetometer
        float lkiMag = 0.0;                					// integral gain governs rate of convergence of gyroscope biases
        float lhalfT = 0.0;                					// half the sample period
        float lq0 = 0.0, lq1 = 0.0, lq2 = 0.0, lq3 = 0.0;   // quaternion elements representing the estimated orientation
        float lexInt = 0.0, leyInt = 0.0, lezInt = 0.0;  	// scaled integral error
};

#endif /* Heading.h */
