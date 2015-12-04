/*
LIDAR PINS
D0 - I2C SDA
D1 - I2C SCL
D2 - LIDAR1 ENABLE
D3 - LIDAR2 ENABLE
*/

#ifndef INC_LIDAR_H_
#define INC_LIDAR_H_

#include "Particle.h"

#define LIDARLITE_ADDR 0x62
#define LIDARLITE_CMD_CTRL_ADDR 0x00
#define LIDARLITE_TRIG_VAL 0x04
#define LIDARLITE_RANGE_ADDR 0x8f
#define LIDAR_SAMPLING_PERIOD 5

extern const uint8_t PIN_LIDAR_EN[2];
extern volatile uint16_t lidar_distance[2];
extern uint8_t enabled_lidar;
extern uint8_t lidar_state;		 // {0:Disabled, 1:Enabled, 2:Triggered, 3:Requested data}
extern uint8_t state_cycle_count;
extern Timer timer_lidar;

void init_lidar();
void cb_lidar();

#endif