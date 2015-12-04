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
#include "ir/ir.h"
#include "us/us.h"
#include "lidar/lidar.h"
#include "imu/imu.h"
#include "controller/controller.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

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