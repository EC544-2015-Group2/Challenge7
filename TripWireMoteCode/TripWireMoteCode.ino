/* MOTE CODE TO CREATE TRIP WIRES
 *  MAKE SURE TO CHANGE THE MSG_TRIP BYTE 0xB1 is mote 1, 0xB2 is mote to and so on

*/
#include <XBee.h>
#include <SoftwareSerial.h>
//
//#define PIN_LED_TRIP1   11
//#define PIN_LED_TRIP2   12

const int PIN_LED_TRIP = 11;
const int PIN_LED_TIMEOUT = 12;

const int us_trig = 4;
const int us_echo = 5;

// LOOP COUNTERS
int count = 0;
long timeStamp = 0;
bool trip = false;
bool prevState = trip;

// SET UP XBEE
const uint8_t MSG_TRIP = 0xB3;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(2, 3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

//SET UP ULTRASONICS
int us_initDistance;
int us_distance;

void setup() {
  Serial.begin(57600);
  xbeeSerial.begin(57600);
  xbee.begin(xbeeSerial);
  delay(2000);
  initUS();
  initLEDPINS();
  for (int i = 0; i < 5; i++) {
    us_ping();
    us_initDistance = us_initDistance + us_distance;
  }
  us_initDistance = us_initDistance / 5; 
  Serial.print("initDist"); Serial.print(": "); Serial.println(us_initDistance);
}

void loop() {
  us_ping();
  evalPing();
}

void us_ping(void) {
  digitalWrite(us_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(us_trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(us_trig, LOW);
  delayMicroseconds(2);
  us_distance = (pulseIn(us_echo, HIGH, 18000)) / 58;
  Serial.println(us_distance);
  delay(100);
}


void sendCommand(uint32_t destinationAddress64, uint8_t* payload, uint8_t length) {
  txRequest = ZBTxRequest(XBeeAddress64(0x00000000, 0x00000000), payload, length);
  xbee.send(txRequest);
  Serial.println("                MESSAGE SENT");   
}

void initLEDPINS(void) {
  pinMode(PIN_LED_TRIP, OUTPUT);
  pinMode(PIN_LED_TIMEOUT, OUTPUT);
  digitalWrite(PIN_LED_TRIP, LOW);
  digitalWrite(PIN_LED_TIMEOUT, LOW);
  
}

void initUS(void) {
  pinMode(us_trig, OUTPUT);
  pinMode(us_echo, INPUT);
  digitalWrite(us_echo, HIGH);
}

void evalPing(void) {
  if (us_distance < 0.75 * us_initDistance) {
    count++;
    if (count == 3) {
      trip = true;
      digitalWrite(PIN_LED_TRIP, HIGH);
      sendCommand(0x00000000, (uint8_t*)&MSG_TRIP, 1);
      delay(2000);
      prevState = trip;
      timeStamp = millis();
      Serial.print("timestamp: ");
      Serial.println(timeStamp);
      digitalWrite(PIN_LED_TIMEOUT, HIGH);
    }
  } else {
    trip = false;
    digitalWrite(PIN_LED_TRIP, LOW);
    Serial.println(millis() - timeStamp);
    if (millis() - timeStamp > 2000) digitalWrite(PIN_LED_TIMEOUT, LOW);
    count = 0;
    prevState = trip;
  }
  Serial.println(count);
}
