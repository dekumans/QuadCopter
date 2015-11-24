#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void initBatteryMonitor();
uint8_t checkBattery();
uint16_t *getBatteryLevels();

#ifdef __cplusplus
}
#endif

#endif /*BatteryMonitor.hpp*/
