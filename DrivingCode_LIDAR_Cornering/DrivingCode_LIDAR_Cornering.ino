#include <math.h>
#include <PID_v1.h>
#include <Servo.h>
#include <Wire.h>
#include <XBee.h>
#include <SoftwareSerial.h>

#include "lidar.h"
#include "encoder.h"
#include "controller.h"
#include "ultrasonic.h"
#include "corner_detection.h"

#define XBEE_MSG_TRIP1   0xB1
#define PIN_LED_MSG 10

#define SERIAL_MSG_FORWARD        0x00
#define SERIAL_MSG_BACKWARD       0x01
#define SERIAL_MSG_TURN_LEFT      0x02
#define SERIAL_MSG_TURN_RIGHT     0x03
#define SERIAL_MSG_TOGGLE_MANUAL  0x04
#define SERVO_INCREMENT  5

uint32_t led_timeout = millis();
uint8_t trigger;
bool turn_flag = false;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(6, 7);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

void setup() {
  pinMode(PIN_LED_MSG, OUTPUT);
  digitalWrite(PIN_LED_MSG, LOW);

  Serial.begin(57600);
  xbeeSerial.begin(57600);
  xbee.begin(xbeeSerial);
  delay(1000);
  
  init_lidar();
  init_encoder();
  init_controller();
  init_us();
  init_cd();

  boolean initialized = false;
  while (!initialized) {
    if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rxResponse);
//      Serial.println(rxResponse.getData(0));
      switch (rxResponse.getData(0)) {
        case 0xB5:
          trigger = 0xB2;
          initialized = true;
          break;
        case 0xB6:
          trigger = 0xB3;
          initialized = true;
          break;
        case 0xB7:
          trigger = 0xB4;
          initialized = true;
          break;
        case 0xB8:
          trigger = 0xB1;
          initialized = true;
          break;
      }
    }
  }
  Serial.print("Initialized: 0x");
  Serial.println(trigger, HEX);
}

void loop() {
  read_lidar();
  compute_controller();
  ping_us();
  ping_cd();
  readAndHandlePackets();
  turnCorner();
  encoder_debounce();
  encoder_logger(trigger);
  if (millis() - led_timeout > 1000) digitalWrite(PIN_LED_MSG, LOW);
  delay(1);
}

//---------------------------------------------------------//

void readAndHandlePackets() {
  if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
    xbee.getResponse().getZBRxResponse(rxResponse);
//    Serial.print("Got a packet!");
    digitalWrite(PIN_LED_MSG, HIGH);
    led_timeout = millis();
    if (rxResponse.getData(0) == trigger) {
      turn_flag = true;
//      Serial.print("Triggered: 0x");
//      Serial.println(trigger, HEX);
      if (trigger < XBEE_MSG_TRIP1 + 3) trigger++;
      else trigger = XBEE_MSG_TRIP1;
    }
  }
}

void turnCorner() {
  if (cd_flag && turn_flag) {
    encoder_count = 0;
    Serial.println("CORNER_DETECTION: TURN");
    for (int i = 0; i < 10; i++) {
      while (us_dist < 50) {
        ping_us();
        motorServo.write(90);
        delay(40);
      }
      motorServo.write(60);
      steeringServo.write(150);
      delay(200);
    }
    turn_flag = false;
    cd_flag = false;
  }
}

void serialEvent(){
  while(Serial.available()){
    switch(Serial.read()){
      case SERIAL_MSG_TOGGLE_MANUAL:
        state_paused = !state_paused;
        if(state_paused){
          motorServo.write(90);
          steeringServo.write(90);
        }
        break;
      case SERIAL_MSG_FORWARD:
        motorServo.write(motorServo.read() - SERVO_INCREMENT);
        break;
      case SERIAL_MSG_BACKWARD:
        motorServo.write(motorServo.read() + SERVO_INCREMENT);
        break;
      case SERIAL_MSG_TURN_LEFT:
        steeringServo.write(steeringServo.read() + SERVO_INCREMENT);
        break;
      case SERIAL_MSG_TURN_RIGHT:
        steeringServo.write(steeringServo.read() - SERVO_INCREMENT);
        break;
    }
  }
}
