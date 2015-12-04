/*
LIDAR PINS
D0 - I2C SDA
D1 - I2C SCL
D2 - LIDAR1 ENABLE
D3 - LIDAR2 ENABLE
*/


#include "Particle.h"
#include "lidar.h"

const uint8_t PIN_LIDAR_EN[] = {D2, D3};
volatile uint16_t lidar_distance[] = {0, 0};
uint8_t enabled_lidar = 0;
uint8_t lidar_state = 0;
uint8_t state_cycle_count = 0;
Timer timer_lidar(LIDAR_SAMPLING_PERIOD, cb_lidar);


void init_lidar(){
	for(int i=0; i<2; i++)	pinMode(PIN_LIDAR_EN[i], OUTPUT);
	timer_lidar.start();
}
void cb_lidar(){
	switch (lidar_state) {
    case 0:
      for (int i = 0; i < 2; i++) digitalWrite(PIN_LIDAR_EN[i], LOW);
      digitalWrite(PIN_LIDAR_EN[enabled_lidar], HIGH);
      lidar_state = 1;
      state_cycle_count = 0;
      break;
    case 1:
      Wire.beginTransmission(LIDARLITE_ADDR);
      Wire.write(LIDARLITE_CMD_CTRL_ADDR);
      Wire.write(LIDARLITE_TRIG_VAL);
      if (Wire.endTransmission() == 0) lidar_state = 2;
      else state_cycle_count++;
      break;
    case 2:
      Wire.beginTransmission(LIDARLITE_ADDR);
      Wire.write(LIDARLITE_RANGE_ADDR);
      if (Wire.endTransmission() == 0) lidar_state = 3;
      else state_cycle_count++;
      break;
    case 3:
      if (Wire.requestFrom(LIDARLITE_ADDR, 2) >= 2) {
        uint16_t val = Wire.read() << 8 | Wire.read();
        if (val < 500 && val > 10) lidar_distance[enabled_lidar] = 0.2 * lidar_distance[enabled_lidar] + 0.8 * val;

        enabled_lidar = 1 - enabled_lidar;
        lidar_state = 0;
      } else state_cycle_count++;
  }
  if (state_cycle_count > 30) {
    lidar_state = 0;
    enabled_lidar = 1 - enabled_lidar;
  }
}