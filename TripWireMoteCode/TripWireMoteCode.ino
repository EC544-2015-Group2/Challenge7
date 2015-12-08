/* MOTE CODE TO CREATE TRIP WIRES
 *  MAKE SURE TO CHANGE THE MSG_TRIP BYTE 0xB1 is mote 1, 0xB2 is mote to and so on

*/
#include <XBee.h>
#include <SoftwareSerial.h>

#define PIN_LED_TRIP      11
#define PIN_LED_TIMEOUT   12
#define PIN_US_TRIG        4
#define PIN_US_ECHO        5
#define PIN_JUMPER         6

// LOOP COUNTERS
uint8_t count = 0;
uint32_t timeStamp = 0;

// SET UP XBEE
const uint8_t MSG_TRIP = 0xB2;
const uint8_t MSG_INIT = MSG_TRIP + 4;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(2, 3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

unsigned long debounce_timestamp = 0, debounce_delay = 50;
int button_state = LOW, last_button_state = LOW;

//SET UP ULTRASONICS
uint16_t us_initDistance;

void setup() {
  Serial.begin(57600);
  xbeeSerial.begin(57600);
  xbee.begin(xbeeSerial);
  initGPIO();
  delay(2000);
  for (int i = 0; i < 5; i++) us_initDistance += us_ping() / 5;
  Serial.print("initDist"); Serial.print(": "); Serial.println(us_initDistance);
}

void loop() {
  int reading = digitalRead(PIN_JUMPER);
  if (reading != last_button_state)  debounce_timestamp = millis();
  if (millis() - debounce_timestamp > debounce_delay) {
    if (reading != button_state) {
      button_state = reading;
      if (button_state == LOW) {
        txRequest = ZBTxRequest(XBeeAddress64(0x00000000, 0x00000000), (uint8_t*)&MSG_INIT, 1);
        xbee.send(txRequest);
        Serial.println("Init");
      }
    }
  }
  last_button_state = reading;

  if (us_ping() < 0.75 * us_initDistance) {
    count++;
    if (count == 3) {
      digitalWrite(PIN_LED_TRIP, HIGH);
      sendCommand();
      timeStamp = millis();
      Serial.print("timestamp: ");
      Serial.println(timeStamp);
      digitalWrite(PIN_LED_TIMEOUT, HIGH);
    }
  } else {
    digitalWrite(PIN_LED_TRIP, LOW);
    Serial.println(millis() - timeStamp);
    if (millis() - timeStamp > 2000) digitalWrite(PIN_LED_TIMEOUT, LOW);
    count = 0;
  }
  Serial.println(count);
  delay(30);
}

uint16_t us_ping(void) {
  digitalWrite(PIN_US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_US_TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_US_TRIG, LOW);
  return pulseIn(PIN_US_ECHO, HIGH, 18000) / 58;
}

void sendCommand() {
  txRequest = ZBTxRequest(XBeeAddress64(0x00000000, 0x00000000), (uint8_t*)&MSG_TRIP, 1);
  //  txRequest.setOption(0x01); // Disable retries and route repair
  xbee.send(txRequest);
  Serial.println("                TRIP MESSAGE SENT");
}

void initGPIO(void) {
  pinMode(PIN_US_TRIG, OUTPUT);
  pinMode(PIN_US_ECHO, INPUT);
  digitalWrite(PIN_US_ECHO, HIGH);

  pinMode(PIN_LED_TRIP, OUTPUT);
  pinMode(PIN_LED_TIMEOUT, OUTPUT);
  digitalWrite(PIN_LED_TRIP, LOW);
  digitalWrite(PIN_LED_TIMEOUT, LOW);

  pinMode(PIN_JUMPER, INPUT);
  digitalWrite(PIN_JUMPER, HIGH);

}
