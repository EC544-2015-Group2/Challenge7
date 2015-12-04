#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "Particle.h"

#define CONTROLLER_OUTPUT_PERIOD 40

extern const uint8_t PIN_MOTOR;
extern const uint8_t PIN_SERVO;
extern Timer timer_controller;

void init_controller();
void cb_controller();

#endif
