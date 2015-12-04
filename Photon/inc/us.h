/*
ULTRASONIC PINS
D4 - uS0 echo (cannot use DAC now)
D5 - uS1 echo (cannot use A1 now)
D6 - uS2 echo
D7 - uS3 echo
*/

#ifndef INC_US_H_
#define INC_US_H_

#define US_SAMPLING_PERIOD 40
#define US_MAX_DELAY 18000

extern const uint8_t PIN_US_ECHO[4];
extern const uint8_t PIN_US_TRIG[4];
extern volatile uint16_t us_distance[4];
extern volatile uint32_t us_start_time[4];
extern volatile uint32_t us_end_time[4];
extern Timer timer_us;

void init_us();
void cb_us();
void us0_isr();
void us1_isr();
void us2_isr();
void us3_isr();

#endif