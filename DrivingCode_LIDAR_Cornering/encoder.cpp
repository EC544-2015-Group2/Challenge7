#include "encoder.h"
volatile uint32_t encoder_count = 0;
uint32_t encoder_distance = 0;
uint32_t timer_encoder = 0;
uint32_t last_count = 0;


void init_encoder() {
  pinMode(PIN_ENCODER_IN, INPUT);
  digitalWrite(PIN_ENCODER_IN, HIGH);
  timer_encoder = millis() + PERIOD_ENCODER;
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_IN), encoder_ISR, RISING);
}

void encoder_ISR() {
  encoder_count++;
}

void encoder_calculate_distance() {
  if (millis() > timer_encoder) {
    timer_encoder = millis() + PERIOD_ENCODER;
    if (encoder_count > last_count) {
      encoder_count = last_count + 1;
      last_count = encoder_count;
    }
    encoder_distance = (encoder_count * 0.625);
    //    Serial.print("Distance: ");
    //    Serial.println(encoder_distance);
    //    Serial.print("Count: ");
//    Serial.println(encoder_count);
  }
}

