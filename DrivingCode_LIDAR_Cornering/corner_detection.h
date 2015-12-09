#ifndef INC_CORNER_DETECTION_H_
#define INC_CORNER_DETECTION_H_

#if ARDUINO >= 100
  #include "Arduino.h"
#else 
  #include "WProgram.h"
#endif

#define PIN_CD_TRIG A0
#define PIN_CD_ECHO A1
#define CD_PERIOD   100

extern uint16_t cd_timer;
extern uint16_t cd_dist;
extern bool cd_flag;

void init_cd();
void ping_cd();

#endif
