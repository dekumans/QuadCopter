#ifndef PID_H
#define PID_H

class PID
{
    public:
        void init(float initP, float initI, float initD);
        float update(float targetPosition, float currentPosition, float timeStep);
        void zeroIntegralError();
        void setPIDfactors(float newP, float newI, float newD);
        void setRampValue(float newValue);
        void printPIDfactors();

    private:
        float P, I, D;
        float windupGuard;
        float rampValue;
        float integratedError;
        float lastError;
        float lastUpdate;
};

#endif /* PID.h */
