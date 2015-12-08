#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#if ARDUINO >= 100
	#include "Arduino.h"
#else 
	#include "WProgram.h"
#endif

#include <math.h>
#include <PID_v1.h>
#include <Servo.h>

#define KP_THETA  1.3
#define KI_THETA  0
#define KD_THETA  0.01
#define KP_DIST   1.2
#define KI_DIST   0.075
#define KD_DIST   0.97
#define KP_VEL    0.5
#define KI_VEL    0
#define KD_VEL    0

#define PIN_SERVO_MOTOR 8
#define PIN_SERVO_STEERING  9
#define CONTROLLER_PERIOD 30

extern Servo steeringServo, motorServo;
extern double set_theta, in_theta, out_theta;
extern double set_dist, in_dist, out_dist;
extern double set_vel, in_vel, out_vel;
extern PID PID_theta, PID_dist, PID_vel;
extern bool state_paused;
extern uint32_t timer_controller;
extern uint32_t encoder_timestamp, encoder_countstamp;

void init_controller();
void compute_controller();
#endif
