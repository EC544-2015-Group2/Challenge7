#include "encoder.h"
volatile boolean encoder_changed = false;
uint32_t encoder_count = 0;
uint32_t encoder_distance = 0;
uint32_t timer_encoder = 0;
uint32_t timer_encoder_debounce = 0;
uint32_t last_count = 0;


void init_encoder() {
  pinMode(PIN_ENCODER_IN, INPUT);
  digitalWrite(PIN_ENCODER_IN, HIGH);
  timer_encoder = millis() + PERIOD_ENCODER;
  timer_encoder_debounce = millis() + PERIOD_ENCODER_DEBOUNCE;
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_IN), encoder_ISR, FALLING);
}

void encoder_ISR() {
  encoder_changed = true;
}

void encoder_logger() {
  if (millis() > timer_encoder) {
    timer_encoder = millis() + PERIOD_ENCODER;
    //    encoder_distance = (encoder_count * 0.625);
    Serial.println(encoder_count);
  }
}

void encoder_debounce() {
  if (millis() > timer_encoder_debounce) {
    timer_encoder_debounce = millis() + PERIOD_ENCODER_DEBOUNCE;
    if (encoder_changed) {
      encoder_count++;
      encoder_changed = false;
    }
  }
}

