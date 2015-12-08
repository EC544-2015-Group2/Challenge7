#include "encoder.h"
volatile uint32_t encoder_count = 0;
uint32_t encoder_distance = 0;
uint32_t timer_encoder = 0;


void init_encoder() {
  pinMode(PIN_ENCODER_IN, INPUT);
  digitalWrite(PIN_ENCODER_IN, HIGH);
  //	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_IN), encoder_ISR, CHANGE);
}

void encoder_ISR() {
  encoder_count++;
}

void encoder_calculate_distance() {
  if (millis() - timer_encoder > 1000) {
    encoder_distance = (encoder_count * 0.625) / 72;
    timer_encoder = millis();
    Serial.print("Distance: ");
    Serial.println(encoder_distance);
  }
}

