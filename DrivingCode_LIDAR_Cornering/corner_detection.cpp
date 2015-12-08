#include "corner_detection.h"

uint16_t cd_dist = 0;
uint16_t cd_timer;
uint8_t count; 
uint8_t max_dist = 200;

void init_cd(){
  pinMode(PIN_CD_TRIG, OUTPUT);
  pinMode(PIN_CD_ECHO, INPUT);
  cd_timer = millis() + CD_PERIOD;
}

bool ping_cd() {
  if(millis() > cd_timer){
    cd_timer = millis() + CD_PERIOD;
    digitalWrite(PIN_CD_TRIG, LOW); delayMicroseconds(2);
    digitalWrite(PIN_CD_TRIG, HIGH);  delayMicroseconds(5);
    digitalWrite(PIN_CD_TRIG, LOW);
    uint16_t val = pulseIn(PIN_CD_ECHO, HIGH, 6000) / 58;
    if (val > max_dist) val = 0;
    if(val == 0) count++;
    if (count == 3) {
      return true;
      count = 0;
    } else return false;
    cd_dist = 0.8 * val + 0.2 * cd_dist;
  }
}
