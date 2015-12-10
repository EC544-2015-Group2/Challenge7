#include "encoder.h"
volatile boolean encoder_changed = false;
uint32_t encoder_count = 0;
//uint32_t encoder_distance = 0;
uint32_t timer_encoder = 0;
uint32_t timer_encoder_debounce = 0;
uint32_t last_count = 0;

// Beacon locations: 120, 348 - 402, 392 - 78, 558 - 358, 602
// Corner locations: 100, 370 - 100, 580 - 380, 580 - 380, 370
// Path corners: 70, 340 - 70, 610 - 410, 610 - 410, 340
uint16_t corner1[] = {70, 340};
uint16_t corner2[] = {70, 610};
uint16_t corner3[] = {410, 610};
uint16_t corner4[] = {410, 340};
uint16_t location[2];
uint16_t encoder_mapped_distance;

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

void encoder_logger(uint8_t trigger) {
  if (millis() > timer_encoder) {
    timer_encoder = millis() + PERIOD_ENCODER;
    //    encoder_distance = (encoder_count * 0.625);
    // encoder_distance = 0.8 * encoder_count * 0.625 / 8;
    encoder_mapped_distance = encoder_count * SCALING_FACTOR;
    switch (trigger) {
      case 0xB1:
        location[0] = corner1[0];
        location[1] = corner1[1] + encoder_mapped_distance;
        break;
      case 0xB2:
        location[0] = corner2[1] + encoder_mapped_distance;
        location[1] = corner2[2];
        break;
      case 0xB3:
        location[0] = corner3[1];
        location[1] = corner3[2] - encoder_mapped_distance;
        break;
      case 0xB4:
        location[0] = corner4[1] - encoder_mapped_distance;
        location[1] = corner4[2];
        break;
    }
    Serial.print(location[0]); Serial.print(", "); Serial.println(location[1]);
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

