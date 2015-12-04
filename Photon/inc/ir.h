/*
IR RANGEFINDER PINS
A0 - IR1
A1 - IR2
*/


#ifndef INC_IR_H_
#define INC_IR_H_

#include "Particle.h"
#include "lib/SharpIR/SharpIR.h"
#define IR_MODEL 20150
#define IR_SAMPLING_PERIOD 50

extern const uint8_t PIN_IR[2];
extern volatile uint16_t ir_distance[2];
extern SharpIR sharp_ir[2];
extern Timer timer_ir;

void init_ir();
void cb_ir();

#endif