#include <Servo.h>
#include <XBee.h>
#include <SoftwareSerial.h>

const uint8_t MSG_ALIVE = 0xB0,
              MSG_TRIP1 = 0xB1,
              MSG_UNTRIP1 = 0xB2,
              MSG_TRIP2 = 0xB3,
              MSG_UNTRIP2 = 0xB4;

Servo wheels; // servo for turning the wheels
Servo esc; // not actually a servo, but controlled like one!
int pos = 90;
int speed = 90;
bool trigger = false;
bool complete = false;
bool threePointStart = false;
int forwardTime = 2000;
int backwardTime = 600;
int pauseTime = 500;
int servoMoveTime = 600;
int straightTime = 1500;
int timeStamp = millis();
bool singleAttempt = true;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(2, 3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

void setup() {
  delay(3000);
  wheels.attach(8); // initialize wheel servo to Digital IO Pin #8
  esc.attach(9); // initialize ESC to Digital IO Pin #9
  wheels.write(pos);
  esc.write(speed);

  Serial.begin(57600);
  xbeeSerial.begin(57600);
  xbee.begin(xbeeSerial);
  delay(2000);
}

void loop() {
  if (singleAttempt) {
    esc.write(60);
    readAndHandlePackets();
    if (trigger && millis() - timeStamp > 5000) {
      Serial.println("Activate early turn");
      while (!complete) {
        left_forward();
        delay(20);
      }
      straight_forward();
      pause_vehicle();
      timeStamp = millis();
      singleAttempt = false;
    }
  } else esc.write(90);
  if (complete) {
    Serial.println("Turn complete");
    complete = false;
  }
}

void left_forward(void) {
  wheels.write(130);
  esc.write(60);
}

void pause_vehicle(void) {
  esc.detach();
  delay(pauseTime);
  esc.attach(9);
  esc.write(90);
}

void straight_forward(void) {
  wheels.write(90);
  delay(servoMoveTime);
  for (speed = 90; speed > 60; speed--) {
    esc.write(speed);
  }
  delay(straightTime);
}

void serialLog(uint8_t payload) {
  switch (payload) {
    case MSG_ALIVE: Serial.println("ALIVE"); break;
    case MSG_TRIP1: Serial.println("TRIP1"); break;
    case MSG_TRIP2: Serial.println("TRIP2"); break;
    case MSG_UNTRIP1: Serial.println("UNTRIP1");  break;
    case MSG_UNTRIP2: Serial.println("UNTRIP2");  break;
  }
}

void readAndHandlePackets(void) {
  if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
    xbee.getResponse().getZBRxResponse(rxResponse);
    serialLog(rxResponse.getData(0));
    switch (rxResponse.getData(0)) {
      case MSG_TRIP1:
        trigger = true;
        complete = false;
        Serial.println(trigger);
        break;
      case MSG_UNTRIP1:
        //
        break;
      case MSG_TRIP2:
        complete = true;
        break;
      case MSG_UNTRIP2:
        //
        
        break;

    }
  }
}
