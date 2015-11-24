#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <kalman.h>

#include "Arduino.h"
#include "Led.h"
#include "BatteryMonitor.h"

kalman_data pitch_data;
kalman_data roll_data;

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro = Adafruit_L3GD20_Unified(20);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 (Accel) detected ... Check your wiring!"));
    while(1);
  }

  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 (Mag) detected ... Check your wiring!");
    while(1);
  }

  gyro.enableAutoRange(true);
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 (Gyro) detected ... Check your wiring!");
    while(1);
  }

  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 (Baro) detected ... Check your wiring!");
    while(1);
  }
}

void printBatteryLevels() {
    uint16_t *levels = getBatteryLevels();
    Serial.print(levels[0], DEC);
    Serial.print(" ");
    Serial.print(levels[1], DEC);
    Serial.print(" ");
    Serial.println(levels[2], DEC);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello");
    initBatteryMonitor();
    initLeds();
    initSensors();
    kalman_init(&pitch_data);
    kalman_init(&roll_data);
}

void loop()
{
    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_event_t gyro_event;
    sensors_event_t bmp_event;
    sensors_vec_t   orientation;

	float acc_x, acc_y, acc_z;
	float gyr_x, gyr_y, gyr_z;
	float acc_pitch, acc_roll;
	float pitch, roll;

    // Read sensor
    accel.getEvent(&accel_event);
    gyro.getEvent(&gyro_event);
    // Calculate pitch and roll based only on acceleration.
    acc_pitch = atan2(accel_event.acceleration.x, accel_event.acceleration.z);
    acc_roll = atan2(accel_event.acceleration.y, accel_event.acceleration.z);
    // Perform filtering
    kalman_innovate(&pitch_data, acc_pitch, gyro_event.gyro.x);
    kalman_innovate(&roll_data, acc_roll, gyro_event.gyro.y);
    // The angle is stored in state 1
    pitch = pitch_data.x1 * 180.0 / 3.14159;
    roll = roll_data.x1 * 180.0 / 3.14159;

    Serial.print("Pitch: ");
    Serial.print(pitch, 2);
    Serial.print(" Roll: ");
    Serial.print(roll, 2);
    Serial.write(27);
    Serial.print("[H");

    ledToggle(0);

    delay(10);
}
