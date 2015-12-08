#include "corner_detection.h"

uint16_t cd_dist = 0;
uint16_t cd_timer;

void init_cd(){
  pinMode(PIN_CD_TRIG, OUTPUT);
  pinMode(PIN_CD_ECHO, INPUT);
  cd_timer = millis() + CD_PERIOD;
}

uint16_t ping_cd() {
  if(millis() > cd_timer){
    cd_timer = millis() + CD_PERIOD;
    digitalWrite(PIN_CD_TRIG, LOW); delayMicroseconds(2);
    digitalWrite(PIN_CD_TRIG, HIGH);  delayMicroseconds(5);
    digitalWrite(PIN_CD_TRIG, LOW);
    uint16_t val = pulseIn(PIN_CD_ECHO, HIGH, 6000) / 58;
    if(val == 0)  return true;
    cd_dist = 0.8 * val + 0.2 * cd_dist;
  }
}
