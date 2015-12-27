#include "Heading.h"

void Heading::init()
{
    if(!mag.init())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println(F("Ooops, no LSM303 (Mag) detected ... Check your wiring!"));
        while(1);
    }

    float aX = 0.0;
    float aY = 0.0;
    float aZ = -9.8;

    float norm = 1, rollAngle, pitchAngle, pi, tmp;
    float yawAngle = atan2(mag.hdgY, mag.hdgX);
    
    norm = sqrt(aX*aX + aY*aY + aZ*aZ);       
    aX = aX / norm;
    aY = aY / norm;
    aZ = aZ / norm;
    
    tmp = atan2(aY, sqrt(aX*aX+aZ*aZ));
    
    if (aZ < 0) { //board up
        tmp = -tmp;
    } else {		//board upside down
        if (aY >= 0) {
            tmp -= PI;
        } else {
            tmp += PI;
        }
    }
    
    rollAngle = tmp;
    pitchAngle = atan2(aX, sqrt(aY*aY+aZ*aZ));
    
    hAngles.roll = -pitchAngle;
    hAngles.pitch = rollAngle;
    hAngles.yaw = yawAngle;
    
    lq0 = cos(rollAngle/2)*cos(pitchAngle/2)*cos(yawAngle/2) + sin(rollAngle/2)*sin(pitchAngle/2)*sin(yawAngle/2);
    lq1 = sin(rollAngle/2)*cos(pitchAngle/2)*cos(yawAngle/2) - cos(rollAngle/2)*sin(pitchAngle/2)*sin(yawAngle/2);
    lq2 = cos(rollAngle/2)*sin(pitchAngle/2)*cos(yawAngle/2) + sin(rollAngle/2)*cos(pitchAngle/2)*sin(yawAngle/2);
    lq3 = cos(rollAngle/2)*cos(pitchAngle/2)*sin(yawAngle/2) - sin(rollAngle/2)*sin(pitchAngle/2)*cos(yawAngle/2);
    
    lexInt = 0.0;
    leyInt = 0.0;
    lezInt = 0.0;
    
    lkpAcc = 2.0; //0.2
    lkiAcc = 0.005; //0.0005
    
    lkpMag = 20.0;//2.0;
    lkiMag = 0.05;//0.005;
}

void Heading::measure(float roll, float pitch)
{
    mag.measure(roll, pitch); 
}

void Heading::calculate(float gx, float gy, float gz,
                        float ax, float ay, float az, float timeStep)
{
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy;
    float q0i, q1i, q2i, q3i;
    float exAcc, eyAcc, ezAcc;
    float ezMag;

    float mx = mag.magData.x;
    float my = mag.magData.y;
    float mz = mag.magData.z;
    
    lhalfT = timeStep/2;
    
    // normalise the measurements
    norm = sqrt(ax*ax + ay*ay + az*az);       
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
    norm = sqrt(mx*mx + my*my + mz*mz);          
    mx = mx / norm;
    my = my / norm;
    mz = mz / norm;         
    
    // compute reference direction of flux
    hx = mx * 2*(0.5 - lq2*lq2 - lq3*lq3) + my * 2*(lq1*lq2 - lq0*lq3)       + mz * 2*(lq1*lq3 + lq0*lq2);
    hy = mx * 2*(lq1*lq2 + lq0*lq3)       + my * 2*(0.5 - lq1*lq1 - lq3*lq3) + mz * 2*(lq2*lq3 - lq0*lq1);
    hz = mx * 2*(lq1*lq3 - lq0*lq2)       + my * 2*(lq2*lq3 + lq0*lq1)       + mz * 2*(0.5 - lq1*lq1 - lq2*lq2);
    
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz;
    
    // estimated direction of gravity and flux (v and w)
    vx = 2*(lq1*lq3 - lq0*lq2);
    vy = 2*(lq0*lq1 + lq2*lq3);
    vz = lq0*lq0 - lq1*lq1 - lq2*lq2 + lq3*lq3;
      
    wx = bx * 2*(0.5 - lq2*lq2 - lq3*lq3) + bz * 2*(lq1*lq3 - lq0*lq2);
    wy = bx * 2*(lq1*lq2 - lq0*lq3)       + bz * 2*(lq0*lq1 + lq2*lq3);
    //wz = bx * 2*(lq0*lq2 + lq1*lq3)       + bz * 2*(0.5 - lq1*lq1 - lq2*lq2);
    
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    exAcc = (vy*az - vz*ay);
    eyAcc = (vz*ax - vx*az);
    ezAcc = (vx*ay - vy*ax);
    
    ezMag = (mx*wy - my*wx);
    
    // integral error scaled integral gain
    lexInt = lexInt + exAcc*lkiAcc;
    leyInt = leyInt + eyAcc*lkiAcc;
    lezInt = lezInt + ezAcc*lkiAcc;
    
    // adjusted gyroscope measurements
    gx = gx + lkpAcc*exAcc + lexInt;
    gy = gy + lkpAcc*eyAcc + leyInt;
    gz = gz + lkpAcc*ezAcc + ezMag*lkpMag + lezInt;
    
    // integrate quaternion rate and normalise
    q0i = (-lq1*gx - lq2*gy - lq3*gz) * lhalfT;
    q1i = ( lq0*gx + lq2*gz - lq3*gy) * lhalfT;
    q2i = ( lq0*gy - lq1*gz + lq3*gx) * lhalfT;
    q3i = ( lq0*gz + lq1*gy - lq2*gx) * lhalfT;
    lq0 += q0i;
    lq1 += q1i;
    lq2 += q2i;
    lq3 += q3i;
    
    // normalise quaternion
    norm = sqrt(lq0*lq0 + lq1*lq1 + lq2*lq2 + lq3*lq3);
    lq0 = lq0 / norm;
    lq1 = lq1 / norm;
    lq2 = lq2 / norm;
    lq3 = lq3 / norm;
    
    hAngles.pitch = atan2(2 * (lq0*lq1 + lq2*lq3), 1 - 2 *(lq1*lq1 + lq2*lq2));
    hAngles.roll = -asin(2 * (lq0*lq2 - lq1*lq3));
    hAngles.yaw = atan2(2 * (lq0*lq3 + lq1*lq2), 1 - 2 *(lq2*lq2 + lq3*lq3));

    trueNorthHeading = hAngles.yaw;

    /*
    trueNorthHeading -= (PI / 2);

    if (trueNorthHeading < 0) {
        trueNorthHeading = (2.0 * PI) + trueNorthHeading;
    }
    */
}

void Heading::printAngles()
{
    Serial.print("Pitch ");
    Serial.print(hAngles.pitch * 180.0 / PI, 0);
    Serial.print(" Roll ");
    Serial.print(hAngles.roll * 180.0 / PI, 0);
    Serial.print(" Yaw ");
    Serial.println(trueNorthHeading * 180.0 / PI, 0);
}

void Heading::printMagData()
{
    Serial.print("Mx: ");
    Serial.print(mag.magData.x, 2);
    Serial.print(" My: ");
    Serial.print(mag.magData.y, 2);
    Serial.print(" Mz: ");
    Serial.println(mag.magData.z, 2);
}
