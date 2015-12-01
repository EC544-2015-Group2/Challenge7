/* MOTE CODE TO CREATE TRIP WIRES

ULTRASONIC PINS
D4, D5

*/
#include <XBee.h>
#include <SoftwareSerial.h>

// LOOP COUNTERS
int count = 0;

// SET UP XBEE
const uint8_t MSG_ALIVE = 0xB0,
			  MSG_TRIP1 = 0xB1,
			  MSG_UNTRIP1 = 0xB2,
			  MSG_TRIP2 = 0xB3,
			  MSG_UNTRIP2 = 0xB4;

XBee xbee = XBee();
SoftwareSerial xbeeSerial(2,3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;

//SET UP ULTRASONICS
int us_initDistance[2];
int us_distance[2];

const int us_trig[] = {D4, D5};
const int us_echo[] = {D6, D7};

void setup() {
	Serial.begin(57600);
	xbeeSerial.begin(57600);
	xbee.begin(xbeeSerial);
	delay(2000);
	for(i=0, i<2, i++) {
		pinMode(us_trig[i], OUTPUT);
		pinMode(us_echo[i], INPUT);
		digitalWrite(us_echo[i], HIGH);
	}
	
	sendCommand(0x00000000, (uint8_t*)&MSG_ALIVE, 1);

	us_ping(); 
	for(i=0, i<2, i++){
		us_initDistance[i] = us_distance[i];
	}

}

void loop() { // MAKE TWO COUNTERS - choose the sensor
	us_ping();
	for (i=0, i<2, i++) {
		if (us_distance[i] < 0.75 * us_initDistance[i]) count++;
		if (count == 3) sendCommand(0x00000000, &MSG_TRIP1, 1)

	}
	
}
void us_ping(){
	for (i=0, i<2, i++){
		digitalWrite(us_trig[i], LOW);
		delayMicroseconds(2); 
		digitalWrite(us_trig[i], HIGH);
		delayMicroseconds(5);
		us_distance[i] = (pulseIn(us_echo[i], HIGH, 18000))/58;
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