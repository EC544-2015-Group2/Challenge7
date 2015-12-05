#include <Servo.h>
#include <XBee.h>
#include <SoftwareSerial.h>

const uint8_t MSG_TRIP1 = 0xB1,
              MSG_TRIP2 = 0xB2,
              MSG_TRIP3 = 0xB3,
              MSG_TRIP4 = 0xB4;

Servo wheels; 
Servo esc; 
int pos = 90;
int speed = 90;
bool trigger[] = {true, false, false, false, false}; // first true is to allow first beacon to trip
bool complete = false;
int forwardTime = 2000;
int pauseTime = 500;
int servoMoveTime = 600;
unsigned long timeStamp = millis();
int count = 1;

// delete singleAttempt in final 
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
    if (trigger[count]) {
      Serial.println("Turn!");
      left_forward();
      timeStamp = millis();
      singleAttempt = false;
      count++;
    }
    
  } else pause_vehicle();
  Serial.println(count);
  delay(30);
}

void left_forward(void) {
  wheels.write(150);
  esc.write(60);
  delay(forwardTime);
}

void pause_vehicle(void) {
  esc.detach();
  delay(pauseTime);
  esc.attach(9);
  esc.write(90);
}

void readAndHandlePackets(void) {
  if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
    xbee.getResponse().getZBRxResponse(rxResponse);
    Serial.print("Got a packet!");
    switch (rxResponse.getData(0)) {
      case MSG_TRIP1:
        if (trigger [0], !trigger[1], !trigger[2], !trigger[3], !trigger[4]) {
          trigger[1] = true;
          Serial.println("TRIP1");
        } 
        break;
      case MSG_TRIP2:
        if (trigger [0], trigger[1], !trigger[2], !trigger[3], !trigger[4]) {
          trigger[2] = true;
          Serial.println("TRIP2");
        }
        break;
      case MSG_TRIP3:
        if (trigger [0], trigger[1], trigger[2], !trigger[3], !trigger[4]) {
          trigger[3] = true;
          Serial.println("TRIP3");
        }
        break;
      case MSG_TRIP4:
        if (trigger [0], trigger[1], trigger[2], trigger[3], !trigger[4]) {
          trigger[4] = true;
          Serial.println("TRIP4");
          complete = true;
        }
        break;

    }
  }
}
