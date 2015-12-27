#include "AHRS.h"

void AHRS::init()
{
    if(!accel.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Accel) detected ... Check your wiring!"));
        while(1);
    }

    if(!gyro.init())
    {
        /* There was a problem detecting the L3DH20 ... check your connections */
        Serial.println(F("Ooops, no L3D20 (Gyro) detected ... Check your wiring!"));
        while(1);
    }

    calibrate();

    q0 = 1.0;
    q1 = 0.0;
    q2 = 0.0;
    q3 = 0.0;
    exInt = 0.0;
    eyInt = 0.0;
    ezInt = 0.0;

    previousEx = 0.0;
    previousEx = 0.0;
    previousEx = 0.0;

    Kp = 1.5;
    Ki = 0.005;

    angles.pitch = 0.0;
    angles.roll = 0.0;
    angles.yaw = 0.0;

    hdg.init();
}

void AHRS::calibrate()
{
    accel.calibrate();
    gyro.calibrate();
}

void AHRS::argUpdate(float gx, float gy, float gz,
                     float ax, float ay, float az, float timeStep)
{
    float norm;
    float vx, vy, vz;
    float q0i, q1i, q2i, q3i;
    float ex, ey, ez;

    halfT = timeStep/2;

    // normalise the measurements
    norm = sqrt(ax*ax + ay*ay + az*az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // error is sum of cross product between reference direction of fields and
    // direction measured by sensors
    ex = (vy*az - vz*ay);
    ey = (vz*ax - vx*az);
    ez = (vx*ay - vy*ax);

    // integral error scaled integral gain
    exInt = exInt + ex*Ki;
    if (isSwitched(previousEx,ex)) {
        exInt = 0.0;
    }
    previousEx = ex;

    eyInt = eyInt + ey*Ki;
    if (isSwitched(previousEy,ey)) {
        eyInt = 0.0;
    }
    previousEy = ey;

    ezInt = ezInt + ez*Ki;
    if (isSwitched(previousEz,ez)) {
        ezInt = 0.0;
    }
    previousEz = ez;

    // adjusted gyroscope measurements
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    // integrate quaternion rate and normalise
    q0i = (-q1*gx - q2*gy - q3*gz) * halfT;
    q1i = ( q0*gx + q2*gz - q3*gy) * halfT;
    q2i = ( q0*gy - q1*gz + q3*gx) * halfT;
    q3i = ( q0*gz + q1*gy - q2*gx) * halfT;
    q0 += q0i;
    q1 += q1i;
    q2 += q2i;
    q3 += q3i;

    // normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;
}

bool AHRS::isSwitched(float previousError, float currentError)
{
    if((previousError > 0 && currentError < 0) ||
        (previousError < 0 && currentError > 0)) {
        return true;
    }
    return false;
}

void AHRS::updateAngles(float timeStep)
{
    float aX = accel.x.evaluate();
    float aY = accel.y.evaluate();
    float aZ = accel.z.evaluate();

    aX = accel.x.computeFilter(aX);
    aY = accel.y.computeFilter(aY);
    aZ = accel.z.computeFilter(aZ);

    float gX = gyro.x.evaluate();
    float gY = gyro.y.evaluate();
    float gZ = gyro.z.evaluate();

    gX *= 0.070f * PI / 180.0f;
    gY *= 0.070f * PI / 180.0f;
    gZ *= 0.070f * PI / 180.0f;

    gyro.x.lastValue = gX;
    gyro.y.lastValue = gY;
    gyro.z.lastValue = gZ;

    argUpdate(gX, gY, gZ, -aX, -aY, -aZ, timeStep);

    angles.pitch = atan2(2 * (q0*q1 + q2*q3), 1 - 2 * (q1*q1 + q2*q2));
    angles.roll = asin(2 * (q0*q2 - q1*q3));
    //angles.yaw = atan2(2 * (q0*q3 + q1*q2), 1 - 2 * (q2*q2 + q3*q3));

    angles.pitch *= 180.0f / PI;
    angles.roll *= -180.0f / PI;
    //angles.yaw *= -180.0f / PI;

}

void AHRS::calculateHeading(float timeStep)
{
    hdg.measure(angles.roll, angles.pitch);
    hdg.calculate(gyro.x.lastValue, gyro.y.lastValue, gyro.z.lastValue,
                  -accel.x.lastFValue, -accel.y.lastFValue, -accel.z.lastFValue, timeStep);
}

void AHRS::readCriticalSensors()
{
    accel.read();
    gyro.read();
}

void AHRS::printAngles()
{
    Serial1.print("Angles");
    Serial1.print(";");
    Serial1.print(angles.pitch, 4);
    Serial1.print(";");
    Serial1.print(angles.roll, 4);

    float yaw = hdg.trueNorthHeading - (PI / 2);
    if (yaw < 0) {
        yaw = (2.0 * PI) + yaw;
    }
    Serial1.print(";");
    Serial1.println(yaw * 180.0 / PI, 4);
}

void AHRS::printSensorData(int a, int m, int g)
{
    if (a != 0) {
        Serial.print("Ax: ");
        Serial.print(accel.accelData.x, 2);
        Serial.print(" Ay: ");
        Serial.print(accel.accelData.y, 2);
        Serial.print(" Az: ");
        Serial.println(accel.accelData.z, 2);
    }

    if (m != 0) {
        hdg.printMagData();
    }

    if (g != 0) {
        Serial.print("Gx: ");
        Serial.print(gyro.gyroData.x, 2);
        Serial.print(" Gy: ");
        Serial.print(gyro.gyroData.y, 2);
        Serial.print(" Gz: ");
        Serial.println(gyro.gyroData.z, 2);
    }
}
