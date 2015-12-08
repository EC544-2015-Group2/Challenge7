#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#if ARDUINO >= 100
	#include "Arduino.h"
#else 
	#include "WProgram.h"
#endif

#define PIN_US_TRIG	4
#define PIN_US_ECHO	5
#define US_PERIOD 	100

extern uint16_t us_timer;
extern uint16_t us_dist;

void init_us();
uint16_t ping_us();

#endif
