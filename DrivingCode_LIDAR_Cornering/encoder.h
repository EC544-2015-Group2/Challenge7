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
#define PERIOD_ENCODER 100
#define PERIOD_ENCODER_DEBOUNCE 50
#define SCALING_FACTOR    1

extern volatile boolean encoder_changed;
extern uint32_t encoder_count;
extern uint32_t encoder_distance;
extern uint32_t timer_encoder;
extern uint32_t last_count;
extern uint16_t corner1[2];
extern uint16_t corner2[2];
extern uint16_t corner3[2];
extern uint16_t corner4[2];
extern uint16_t location[2];
extern uint16_t encoder_mapped_distance;

void init_encoder();
void read_encoder();
void encoder_ISR();
void encoder_debounce();
void encoder_logger(uint8_t);

#endif
