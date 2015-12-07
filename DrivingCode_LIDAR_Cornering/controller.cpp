#include "controller.h"
#include "lidar.h"
#include "encoder.h"
#include "ultrasonic.h"

Servo steeringServo, motorServo;
double set_theta = 0, in_theta, out_theta;
double set_dist = 90, in_dist, out_dist;
double set_vel = 60, in_vel, out_vel;
PID PID_theta(&in_theta, &out_theta, &set_theta, KP_THETA, KI_THETA, KD_THETA, DIRECT);
PID PID_dist(&in_dist, &out_dist, &set_dist, KP_DIST, KI_DIST, KD_DIST, DIRECT);
PID PID_vel(&in_vel, &out_vel, &set_vel, KP_VEL, KI_VEL, KD_VEL, DIRECT);
bool state_paused = false;
uint32_t encoder_timestamp, encoder_countstamp;
uint32_t timer_controller;

void init_controller() {
  PID_theta.SetMode(AUTOMATIC);
  PID_dist.SetMode(AUTOMATIC);
  PID_vel.SetMode(AUTOMATIC);

  PID_theta.SetOutputLimits(-45, 45);
  PID_dist.SetOutputLimits(-45, 45);
  PID_vel.SetOutputLimits(-20, 20);

  steeringServo.attach(PIN_SERVO_STEERING); // initialize wheel servo to Digital IO Pin #8
  motorServo.attach(PIN_SERVO_MOTOR); // initialize motorServo to Digital IO Pin #9
  steeringServo.write(90);
  motorServo.write(90);

  timer_controller = millis() + CONTROLLER_PERIOD;
}

void compute_controller() {
  if (!state_paused && millis() > timer_controller) {
    if (us_dist < 50) {
      motorServo.write(90);
      delay(1000);
    }
    else {
      timer_controller = millis() + CONTROLLER_PERIOD;
      in_theta = atan((lidar_dist[0] - lidar_dist[1]) / LIDAR_SPACING);
      in_dist = (lidar_dist[0] + lidar_dist[1]) * cos(in_theta) / 2;
      in_theta *= 180 / PI;
      in_vel = ENCODER_SCALING * (encoder_count - encoder_countstamp) / (millis() - encoder_timestamp);

      PID_theta.Compute();
      PID_dist.Compute();
      PID_vel.Compute();

      motorServo.write(70);
      steeringServo.write(90 + constrain(out_theta - out_dist, -45, 45));
    }
  }
}
