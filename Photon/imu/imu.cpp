#include "Particle.h"
#include "imu.h"

Adafruit_BNO055 bno = Adafruit_BNO055();
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
