#ifndef INC_LIDAR_H_
#define INC_LIDAR_H_

#if ARDUINO >= 100
	#include "Arduino.h"
#else 
	#include "WProgram.h"
#endif
#include <Wire.h>

#define LIDARLITE_ADDR              0x62
#define LIDARLITE_CMD_CTRL_ADDR     0x00
#define LIDARLITE_TRIG_VAL          0x04
#define LIDARLITE_RANGE_ADDR        0x8f
#define LIDAR_SPACING 				22.5

extern const uint8_t PIN_LIDAR_EN[2];	// PWR_EN pins to put lidars to sleep
extern uint8_t enabled_lidar;
extern uint8_t lidar_state;    // {0:Disabled, 1:Enabled, 2:Triggered, 3:Requested data}
extern uint8_t state_cycle_count;
extern double lidar_dist[2];

void init_lidar();
void read_lidar();

#endif