#include "encoder.h"
volatile uint32_t encoder_count = 0;

void init_encoder(){
	pinMode(PIN_ENCODER_IN, INPUT);
	digitalWrite(PIN_ENCODER_IN, HIGH);
//	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_IN), encoder_ISR, CHANGE);
}

void encoder_ISR() {
	encoder_count++;
}
