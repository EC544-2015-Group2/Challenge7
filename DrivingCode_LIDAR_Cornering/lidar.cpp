#include "lidar.h"
const uint8_t PIN_LIDAR_EN[] = {12, 13};    
uint8_t enabled_lidar = 0;
uint8_t lidar_state = 0;
uint8_t state_cycle_count = 0;
double lidar_dist[2];

void init_lidar(){
	// Open and join irc bus as master
	Wire.begin();
	delay(100);

	// Set up the lidar pins and change the lidar acquisition count register value
	for (int i = 0; i < 2; i++)   pinMode(PIN_LIDAR_EN[i], OUTPUT);
	for (int i = 0; i < 2; i++) {
	  for (int j = 0; j < 2; j++)  digitalWrite(PIN_LIDAR_EN[j], LOW);
	  digitalWrite(PIN_LIDAR_EN[i], HIGH);
	  while (1) {
	    Wire.beginTransmission(LIDARLITE_ADDR);
	    Wire.write(0x01);
	    Wire.write(0xFF);
	    if (Wire.endTransmission() == 0) break;
	  }
	}
}

void read_lidar() {
//  Serial.print(lidar_state);
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
        if (enabled_lidar == 0) val = 6.5 + 0.98 * val;
        if (val < 500 && val > 10) lidar_dist[enabled_lidar] = 0.2 * lidar_dist[enabled_lidar] + 0.8 * val;
//        Serial.println(' ');
        enabled_lidar = 1 - enabled_lidar;
        lidar_state = 0;
      } else state_cycle_count++;
      break;
  }
  if (state_cycle_count > 30) {
    lidar_state = 0;
    enabled_lidar = 1 - enabled_lidar;
    state_cycle_count = 0;
//    Serial.println("RST"); 
  }
}
