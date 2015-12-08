#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#if ARDUINO >= 100
	#include "Arduino.h"
#else 
	#include "WProgram.h"
#endif

#define PIN_ENCODER_IN	2
#define ENCODER_SCALING 833.33 // Convert from pulses/ms to rpm 60000/72
#define PULSE_TO_DISTANCE

extern volatile uint32_t encoder_count;
extern uint32_t encoder_distance;
extern uint32_t timer_encoder;

void init_encoder();
void read_encoder();
void encoder_ISR();
void encoder_calculate_distance();

#endif
