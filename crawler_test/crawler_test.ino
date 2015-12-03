#include <Servo.h>
int startupDelay = 1000; // time to pause at each calibration step
Servo wheels; // servo for turning the wheels
Servo esc; // not actually a servo, but controlled like one!
int pos = 90;

void setup() {
  wheels.attach(8); // initialize wheel servo to Digital IO Pin #8
  esc.attach(9); // initialize ESC to Digital IO Pin #9
  wheels.write(90);
}

void loop() {
    smooth_left_turn();
    delay(startupDelay);
    smooth_right_turn();
    delay(startupDelay);
}

void smooth_left_turn(){
  wheels.write(60);
  for(;pos>=60;pos-=1){
    esc.write(pos);
    delay(50);
  }
}

void smooth_right_turn(){
  wheels.write(120);
  for(;pos<=120;pos+=1){
    esc.write(pos);
    delay(50);
  }
}


