/*
IR RANGEFINDER PINS
A0 - IR1
A1 - IR2
*/

#include "Particle.h"
#include "ir.h"

const uint8_t PIN_IR[] = {A0, A1};
volatile uint16_t ir_distance[] = {0, 0};
SharpIR sharp_ir[] = {SharpIR(PIN_IR[0], IR_MODEL), SharpIR(PIN_IR[1], IR_MODEL)};
Timer timer_ir(IR_SAMPLING_PERIOD, cb_ir);

void init_ir(){
	for(int i=0; i<2; i++)	pinMode(PIN_IR[i], INPUT);
	timer_ir.start();
}
void cb_ir() {
	for(int i=0; i<2; i++) ir_distance[i] = sharp_ir[i].distance();
	// Serial.print(ir_distance[0]);	Serial.print("\t");		Serial.println(ir_distance[1]);
	Serial.println(analogRead(PIN_IR[0]));
}
