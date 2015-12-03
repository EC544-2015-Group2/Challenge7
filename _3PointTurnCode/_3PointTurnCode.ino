#include <Servo.h>

Servo wheels; // servo for turning the wheels
Servo esc; // not actually a servo, but controlled like one!
int pos = 90;
int speed = 90;
int count = 0;
bool trigger = true;
int forwardTime = 1000;
int backwardTime = 600;
int pauseTime = 500;
int servoMoveTime = 600;
int straightTime = 1500;

void setup() {
  delay(3000);
  wheels.attach(8); // initialize wheel servo to Digital IO Pin #8
  esc.attach(9); // initialize ESC to Digital IO Pin #9
  wheels.write(pos);
  esc.write(speed);
}

void loop() {
  if (trigger) {
    while (count < 1) {
      left_forward();
      pause_vehicle();
      right_backward();
      pause_vehicle();
      straight_forward();
      pause_vehicle();
      count++  ;
    }
  }
}

void left_forward(void) {
  wheels.write(130);
  delay(servoMoveTime);
  for (speed = 90; speed > 60; speed--) {
    esc.write(speed);
  }
  delay(forwardTime);
}

void right_backward(void) {
  wheels.write(50);
  delay(servoMoveTime);
  for (speed = 90; speed < 120; speed++) {
    esc.write(speed);
  }
  delay(backwardTime);
}

void pause_vehicle(void) {
  esc.detach();
  delay(pauseTime);
  esc.attach(9);
  esc.write(90);
}

void straight_forward(void) {
  wheels.write(90);
  delay(servoMoveTime);
  for (speed = 90; speed > 60; speed--) {
    esc.write(speed);
  }
  delay(straightTime);
}



