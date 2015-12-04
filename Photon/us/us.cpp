/*
ULTRASONIC PINS
D4 - uS0 echo (cannot use DAC now)
D5 - uS1 echo (cannot use A1 now)
D6 - uS2 echo
D7 - uS3 echo
*/


#include "Particle.h"
#include "us.h"

const uint8_t PIN_US_ECHO[] = {D4, D5, D6, D7};
const uint8_t PIN_US_TRIG[] = {A2, A3, DAC, WKP};
volatile uint16_t us_distance[] = {0, 0, 0, 0};
extern volatile uint32_t us_start_time[] = {0, 0, 0, 0};
extern volatile uint32_t us_end_time[] = {0, 0, 0, 0};
Timer timer_us = Timer(US_SAMPLING_PERIOD, cb_us);

void init_us(){
	for(int i=0; i<4; i++){
		pinMode(PIN_US_TRIG[i], OUTPUT);
		pinMode(PIN_US_ECHO[i], INPUT);
	}
	attachInterrupt(PIN_US_ECHO[0], us0_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[1], us1_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[2], us2_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[3], us3_isr, CHANGE);
	timer_us.start();
}
void cb_us(){
	for(int i=0; i<4; i++)
		if(us_end_time[i] > us_start_time[i])		// Takes care of micros() timer overflow every 35 seconds
			us_distance[i] = (us_end_time[i] - us_start_time[i])/58;
	Serial.println(us_distance[0]);

	// Trigger timing values in microseconds taken from NewPing library
	for(int i=0; i<4; i++)	digitalWrite(PIN_US_TRIG[i], LOW);
	delayMicroseconds(4);
	for(int i=0; i<4; i++)	digitalWrite(PIN_US_TRIG[i], HIGH);
	delayMicroseconds(10);
	for(int i=0; i<4; i++)	digitalWrite(PIN_US_TRIG[i], LOW);

	uint32_t now = micros();
	for(int i=0; i<4; i++)	us_end_time[i] = now + US_MAX_DELAY;
}
void us0_isr(){
	if(pinReadFast(PIN_US_ECHO[0])){
		us_start_time[0] = micros();
		us_end_time[0] = us_start_time[0] + US_MAX_DELAY;
	}
	else	us_end_time[0] = micros();
}
void us1_isr(){
	if(pinReadFast(PIN_US_ECHO[1])){
		us_start_time[1] = micros();
		us_end_time[1] = us_start_time[1] + US_MAX_DELAY;
	}
	else	us_end_time[1] = micros();
}
void us2_isr(){
	if(pinReadFast(PIN_US_ECHO[2])){
		us_start_time[2] = micros();
		us_end_time[2] = us_start_time[2] + US_MAX_DELAY;
	}
	else	us_end_time[2] = micros();
}
void us3_isr(){
	if(pinReadFast(PIN_US_ECHO[3])){
		us_start_time[3] = micros();
		us_end_time[3] = us_start_time[3] + US_MAX_DELAY;
	}
	else	us_end_time[3] = micros();
}