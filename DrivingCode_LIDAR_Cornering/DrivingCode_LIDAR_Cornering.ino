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

#define XBEE_MSG_TRIP1   0xB1
#define PIN_LED_MSG 10

uint32_t led_timeout = millis();
uint8_t trigger;

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
  delay(3000);

  init_lidar();
  init_encoder();
  init_controller();
  init_us();

  while (!xbee.readPacket(1) && xbee.getResponse().getApiId() != ZB_RX_RESPONSE) {};
  xbee.getResponse().getZBRxResponse(rxResponse);
  trigger = rxResponse.getData(0) + 1;
  if(trigger > XBEE_MSG_TRIP1 + 3)  trigger = XBEE_MSG_TRIP1;
  Serial.print("Initialized: 0x");
  Serial.println(trigger, HEX);
}

void loop() {
  read_lidar();
  compute_controller();
  ping_us();
  readAndHandlePackets();
  encoder_calculate_distance();
  if (millis() - led_timeout > 1000) digitalWrite(PIN_LED_MSG, LOW);
  delay(1);
}

//---------------------------------------------------------//

void readAndHandlePackets() {
  if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
    xbee.getResponse().getZBRxResponse(rxResponse);
    Serial.print("Got a packet!");
    digitalWrite(PIN_LED_MSG, HIGH);
    led_timeout = millis();
    if (rxResponse.getData(0) == trigger) {
      Serial.print("Triggered: 0x");
      Serial.println(trigger, HEX);
      if (trigger < XBEE_MSG_TRIP1 + 3) trigger++;
      else trigger = XBEE_MSG_TRIP1;
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
    }
  }
}
