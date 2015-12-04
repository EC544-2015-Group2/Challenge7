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

A2 - uS0 trig
A3 - uS1 trig
DAC - uS2 trig
WKP - uS3 trig

IR RANGEFINDER PINS
A0 - IR1
A1 - IR2
*/

// Semi automatic system mode to allow manual control of WiFi module
#include "Particle.h"
SYSTEM_MODE(SEMI_AUTOMATIC)

/*
 * 			SHARP IR SENSOR
 */

#include "SharpIR/SharpIR.h"
#define IR_MODEL 20150
#define IR_SAMPLING_PERIOD 50
const uint8_t PIN_IR[] = {A0, A1};
volatile uint16_t ir_distance[2];
SharpIR sharp_ir[] = {SharpIR(PIN_IR[0], IR_MODEL), SharpIR(PIN_IR[1], IR_MODEL)};
void init_ir();
void cb_ir();
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




/*
 * 			HC SR04 ULTRASONIC SENSOR
 */
#define US_SAMPLING_PERIOD 40
#define US_MAX_DELAY 18000
const uint8_t PIN_US_ECHO[] = {D4, D5, D6, D7};
const uint8_t PIN_US_TRIG[] = {A2, A3, DAC, WKP};
volatile uint16_t us_distance[4];
volatile uint32_t us_start_time[4];
volatile uint32_t us_end_time[4];
void init_us();
void cb_us();
void us0_isr();
void us1_isr();
void us2_isr();
void us3_isr();
Timer timer_us(US_SAMPLING_PERIOD, cb_us);
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





/*
 * 			LIDARLITE v1 SENSOR
 */
#define LIDARLITE_ADDR 0x62
#define LIDARLITE_CMD_CTRL_ADDR 0x00
#define LIDARLITE_TRIG_VAL 0x04
#define LIDARLITE_RANGE_ADDR 0x8f
#define LIDAR_SAMPLING_PERIOD 5
const uint8_t PIN_LIDAR_EN[2] = {D2, D3};
volatile uint16_t lidar_distance[2];
uint8_t enabled_lidar = 0;
uint8_t lidar_state = 0;		 // {0:Disabled, 1:Enabled, 2:Triggered, 3:Requested data}
uint8_t state_cycle_count = 0;
void init_lidar();
void cb_lidar();
Timer timer_lidar(LIDAR_SAMPLING_PERIOD, cb_lidar);
void init_lidar(){
	for(int i=0; i<2; i++)	pinMode(PIN_LIDAR_EN[i], OUTPUT);
	timer_lidar.start();
}
void cb_lidar(){
	switch (lidar_state) {
    case 0:
      for (int i = 0; i < 2; i++) digitalWrite(PIN_LIDAR_EN[i], LOW);
      digitalWrite(PIN_LIDAR_EN[enabled_lidar], HIGH);
      lidar_state = 1;
      state_cycle_count = 0;
      break;
    case 1:
      Wire.beginTransmission(LIDARLITE_ADDR);
      Wire.write(LIDARLITE_CMD_CTRL_ADDR);
      Wire.write(LIDARLITE_TRIG_VAL);
      if (Wire.endTransmission() == 0) lidar_state = 2;
      else state_cycle_count++;
      break;
    case 2:
      Wire.beginTransmission(LIDARLITE_ADDR);
      Wire.write(LIDARLITE_RANGE_ADDR);
      if (Wire.endTransmission() == 0) lidar_state = 3;
      else state_cycle_count++;
      break;
    case 3:
      if (Wire.requestFrom(LIDARLITE_ADDR, 2) >= 2) {
        uint16_t val = Wire.read() << 8 | Wire.read();
        if (val < 500 && val > 10) lidar_distance[enabled_lidar] = 0.2 * lidar_distance[enabled_lidar] + 0.8 * val;

        enabled_lidar = 1 - enabled_lidar;
        lidar_state = 0;
      } else state_cycle_count++;
  }
  if (state_cycle_count > 30) {
    lidar_state = 0;
    enabled_lidar = 1 - enabled_lidar;
  }
}




/*
 * 			BNO055 9DOF IMU
 */
#include "Adafruit_BNO055/Adafruit_BNO055.h"
#include "Adafruit_BNO055/utility/imumaths.h"
#define IMU_SAMPLING_PERIOD 40
Adafruit_BNO055 bno = Adafruit_BNO055();
void init_imu();
void cb_imu();
Timer timer_imu(IMU_SAMPLING_PERIOD, cb_imu);
imu::Vector<3> euler;
void init_imu(){
	if(!bno.begin()){
		 // There was a problem detecting the BNO055 ... check your connections 
		Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
		while(1);
	}
	delay(1000);
	bno.setExtCrystalUse(true);
	timer_imu.start();
}
void cb_imu(){
	euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
}





/*
 * 			CONTROLLER
 */
#define CONTROLLER_OUTPUT_PERIOD 40
const uint8_t PIN_MOTOR = A4;
const uint8_t PIN_SERVO = A5;
void init_controller();
void cb_controller();
Timer timer_controller(CONTROLLER_OUTPUT_PERIOD, cb_controller);
void init_controller(){
	pinMode(PIN_MOTOR, OUTPUT);
	pinMode(PIN_SERVO, OUTPUT);
	timer_controller.start();
}
void cb_controller(){

}




void setup(){
	Wire.begin();
	Serial.begin(57600);
	// init_imu();
	// init_ir();
	// init_lidar();
	init_us();
	// init_controller();
}

void loop() {}