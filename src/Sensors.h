#ifndef SENSORS_H
#define SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float roll;
    float pitch;
    float yaw;
} angles;

void initSensors();
void updateSensors();
angles getAngles();

#ifdef __cplusplus
}
#endif

#endif /*Sensors.hpp*/
