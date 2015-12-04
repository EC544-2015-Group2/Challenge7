#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "Particle.h"
#include "Adafruit_BNO055/Adafruit_BNO055.h"
#include "Adafruit_BNO055/utility/imumaths.h"
#define IMU_SAMPLING_PERIOD 40

extern Adafruit_BNO055 bno;
extern Timer timer_imu;
extern imu::Vector<3> euler;

void init_imu();
void cb_imu();

#endif