/* MOTE CODE TO CREATE TRIP WIRES

*/
#include <XBee.h>
#include <SoftwareSerial.h>
//
//#define PIN_LED_TRIP1   11
//#define PIN_LED_TRIP2   12

const int PIN_LED_TRIP[] = {11, 12};

// LOOP COUNTERS
int count[] = {0, 0};
bool trip[] = {false, false};
bool prevState[] = {trip[1], trip[2]};

// SET UP XBEE
const uint8_t MSG_ALIVE = 0xB0,
              MSG_TRIP1 = 0xB1,
              MSG_UNTRIP1 = 0xB2,
              MSG_TRIP2 = 0xB3,
              MSG_UNTRIP2 = 0xB4;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(2, 3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

//SET UP ULTRASONICS
int us_initDistance[2];
int us_distance[2];

const int us_trig[] = {4, 6};
const int us_echo[] = {5, 7};

void setup() {
  Serial.begin(57600);
  xbeeSerial.begin(57600);
  xbee.begin(xbeeSerial);
  delay(2000);
  initUS();
  initLEDPINS();
  sendCommand(0x00000000, (uint8_t*)&MSG_ALIVE, 1);
  for (int i = 0; i < 5; i++) {
    us_ping();
    for (int i = 0; i < 2; i++) {
      us_initDistance[i] = us_initDistance[i] + us_distance[i];
    }

  }

  for (int i = 0; i < 2; i++) {
    us_initDistance[i] = us_initDistance[i] / 4; //first value is zero for some reason
    Serial.print("initDist"); Serial.print(i); Serial.print(": "); Serial.println(us_initDistance[i]);
  }
}

void loop() {
  us_ping();
  evalPing();
}

void us_ping(void) {
  for (int i = 0; i < 2; i++) {
    digitalWrite(us_trig[i], LOW);
    delayMicroseconds(2);
    digitalWrite(us_trig[i], HIGH);
    delayMicroseconds(5);
    us_distance[i] = (pulseIn(us_echo[i], HIGH, 18000)) / 58;
    Serial.println(us_distance[i]);
    delay(250);
  }
}

void serialLog(bool in, uint32_t address64, uint8_t payload) {
  if (in)  Serial.print("MSG_IN");
  else Serial.print("                                       MSG_OUT");
  Serial.print(":");
  Serial.print(address64, HEX);
  Serial.print(":");
  switch (payload) {
    case MSG_ALIVE: Serial.println("ALIVE"); break;
    case MSG_TRIP1: Serial.println("TRIP1"); break;
    case MSG_TRIP2: Serial.println("TRIP2"); break;
    case MSG_UNTRIP1: Serial.println("UNTRIP1");  break;
    case MSG_UNTRIP2: Serial.println("UNTRIP2");  break;
  }
}

void sendCommand(uint32_t destinationAddress64, uint8_t* payload, uint8_t length) {
  serialLog(false, destinationAddress64, payload[0]);
  txRequest = ZBTxRequest(XBeeAddress64(0x00000000, 0x00000000), payload, length);
  xbee.send(txRequest);
}

void initLEDPINS(void) {
  for (int i=0; i<2; i++) {
    pinMode(PIN_LED_TRIP[i], OUTPUT);
    digitalWrite(PIN_LED_TRIP[i], LOW);
  }
}

void initUS(void) {
  for (int i = 0; i < 2; i++) {
    pinMode(us_trig[i], OUTPUT);
    pinMode(us_echo[i], INPUT);
    digitalWrite(us_echo[i], HIGH);
  }
}

void evalPing(void) {
  for (int i = 0; i < 2; i++) {
    if (us_distance[i] < 0.75 * us_initDistance[i]) {
      count[i]++;
      if (count[i] == 3) {
        trip[i] = true;
        digitalWrite(PIN_LED_TRIP[i], HIGH);
        switch (i) {
          case 0:
            sendCommand(0x00000000, (uint8_t*)&MSG_TRIP1, 1);
            break;
          case 1:
            sendCommand(0x00000000, (uint8_t*)&MSG_TRIP2, 1);
            break;
        }
        prevState[i] = trip[i];
      }
    } else {
      trip[i] = false;
      digitalWrite(PIN_LED_TRIP[i], LOW);
      count[i] = 0;
      if (trip[i] != prevState[i]) {
        switch (i) {
          case 0:
            sendCommand(0x00000000, (uint8_t*)&MSG_UNTRIP1, 1);
            break;
          case 1:
            sendCommand(0x00000000, (uint8_t*)&MSG_UNTRIP2, 1);
            break;
        }
      }
      prevState[i] = trip[i];

    }
    Serial.println(count[i]);
  }

}