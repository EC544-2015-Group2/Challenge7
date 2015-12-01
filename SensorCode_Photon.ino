/*
PIN MAPPINGS

LIDAR PINS
D0 - I2C SDA
D1 - I2C SCL
D2 - LIDAR1 ENABLE
D3 - LIDAR2 ENABLE

XBEE PINS
RX - SERIAL1 RX
TX - SERIAL1 TX

MOTOR/SERVO PWM PINS
A4 - MOTOR
A5 - SERVO

ULTRASONIC PINS
D4 - uS0 echo (cannot use DAC now)
D5 - uS1 echo (cannot use A1 now)
D6 - uS2 echo
D7 - uS3 echo

A2 - uS1 trig
A3 - uS2 trig
DAC - uS3 trig
WKP - uS4 trig

IR RANGEFINDER PINS
A0 - IR1
A1 - IR2
*/

// Semi automatic system mode to allow manual control of WiFi module
SYSTEM_MODE(SEMI_AUTOMATIC)


#include "SharpIR.h"
#define IR_MODEL 20150
#define IR_SAMPLING_PERIOD 50
const uint8_t PIN_IR[] = {A0, A1};
volatile uint16_t ir_distance[2];
void cb_ir() {
	static SharpIR sharp_ir[] = {SharpIR(PIN_IR[0], IR_MODEL), SharpIR(PIN_IR[1], IR_MODEL)};
	for(int i=0; i<2; i++) ir_distance[i] = sharp_ir[i].distance();
}
Timer timer_ir(IR_SAMPLING_PERIOD, cb_ir);


#define US_SAMPLING_PERIOD 40
#define US_MAX_DELAY 18000
const uint8_t PIN_US_ECHO[] = {D4, D5, D6, D7};
const uint8_t PIN_US_TRIG[] = {A2, A3, DAC, WKP};
volatile uint16_t us_distance[4];
volatile uint32_t us_start_time[4];
volatile uint32_t us_end_time[4];
void cb_us(){
	for(int i=0; i<4; i++)
		if(us_end_time[i] > us_start_time[i])		// Takes care of micros() timer overflow every 35 seconds
			us_distance[i] = (us_end_time[i] - us_start_time[i])/58;

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
	if(pinReadFast(PIN_US_ECHO[0])) 	us_start_time[0] = micros();
	else	us_end_time[0] = micros();
}
void us1_isr(){
	if(pinReadFast(PIN_US_ECHO[1])) 	us_start_time[1] = micros();
	else	us_end_time[1] = micros();
}
void us2_isr(){
	if(pinReadFast(PIN_US_ECHO[2])) 	us_start_time[2] = micros();
	else	us_end_time[2] = micros();
}
void us3_isr(){
	if(pinReadFast(PIN_US_ECHO[3])) 	us_start_time[3] = micros();
	else	us_end_time[3] = micros();
}
Timer timer_us(US_SAMPLING_PERIOD, cb_us);


const uint8_t PIN_LIDAR_EN[2] = {D2, D3};
volatile uint16_t lidar_distance[2];


const uint8_t PIN_MOTOR = A4;
const uint8_t PIN_SERVO = A5;



void setup(){
	initIOPins();
	timer_ir.start();

	attachInterrupt(PIN_US_ECHO[0], us0_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[1], us1_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[2], us2_isr, CHANGE);
	attachInterrupt(PIN_US_ECHO[3], us3_isr, CHANGE);
	timer_us.start();
}

void loop() {

}

void initIOPins(){
	for(int i=0; i<2; i++)	pinMode(PIN_IR[i], INPUT);
	for(int i=0; i<2; i++)	pinMode(PIN_LIDAR_EN[i], OUTPUT);
	for(int i=0; i<4; i++){
		pinMode(PIN_US_TRIG[i], OUTPUT);
		pinMode(PIN_US_ECHO[i], INPUT);
	}
	pinMode(PIN_MOTOR, OUTPUT);
	pinMode(PIN_SERVO, OUTPUT);
}
