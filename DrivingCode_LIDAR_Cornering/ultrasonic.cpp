#include "ultrasonic.h"

uint16_t us_dist = 0;
uint16_t us_timer;

void init_us(){
	pinMode(PIN_US_TRIG, OUTPUT);
	pinMode(PIN_US_ECHO, INPUT);
	us_timer = millis() + US_PERIOD;
}

uint16_t ping_us() {
	if(millis() > us_timer){
		us_timer = millis() + US_PERIOD;
		digitalWrite(PIN_US_TRIG, LOW);	delayMicroseconds(2);
		digitalWrite(PIN_US_TRIG, HIGH);	delayMicroseconds(5);
		digitalWrite(PIN_US_TRIG, LOW);
    uint16_t val = pulseIn(PIN_US_ECHO, HIGH, 6000) / 58;
    if(val == 0)  val = 51;
		us_dist = 0.8 * val + 0.2 * us_dist;
	}
}
