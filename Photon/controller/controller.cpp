#include "Particle.h"
#include "controller.h"

const uint8_t PIN_MOTOR = A4;
const uint8_t PIN_SERVO = A5;
Timer timer_controller(CONTROLLER_OUTPUT_PERIOD, cb_controller);

void init_controller(){
	pinMode(PIN_MOTOR, OUTPUT);
	pinMode(PIN_SERVO, OUTPUT);
	timer_controller.start();
}
void cb_controller(){

}