#include "arm.h"

Arm::Arm(MeSmartServo *ss, int handles[2], int lift, int flip) {
  this->handles[0] = handles[0];
  this->handles[1] = handles[1];
  this->lift = new MeDCMotor(lift);
  this->flip = new MeDCMotor(flip);
  this->SmartServo = ss;
  this->SmartServo->setInitAngle(this->handles[0]);
  this->SmartServo->setInitAngle(this->handles[1]);
  this->lift = new MeDCMotor(1);
  this->flip = new MeDCMotor(2);
}

bool Arm::SwitchHand(int id) {
  if ((id == 1 && this->last_open_1) || (id == 2 && this->last_open_2) ||
      (id == 0 && (this->last_open_1 || this->last_open_2)))
    return false;
  if (id == 1 || id == 0) {
    if (!this->isOpen_1) {
      this->SmartServo->setInitAngle(this->handles[0]);
      this->last_open_1 = millis();
      this->isOpen_1 = true;
    } else {
      this->SmartServo->setPwmMove(this->handles[0], 50);
      this->last_open_1 = millis();
      this->isOpen_1 = false;
    }
  }
  if (id == 2 || id == 0) {
    if (!this->isOpen_2) {
      this->SmartServo->setInitAngle(this->handles[1]);
      this->last_open_2 = millis();
      this->isOpen_2 = true;
    } else {
      this->SmartServo->setPwmMove(this->handles[1], -50);
      this->last_open_2 = millis();
      this->isOpen_2 = false;
    }
  }
  return true;
}

bool Arm::SwitchLift(int power) {
  this->lift->run(power);
  /*
  if (this->last_lift)
    return false;
  if (!upward) {
    this->lift->run(-150); // downward
    this->last_lift = millis();
  } else {
    this->lift->run(150); // upward
    this->last_lift = millis();
  }
  */
  return true;
}

bool Arm::SwitchFlip() {
  if (this->last_flip)
    return false;
  if (this->isFilped) {
    this->flip->run(255); // unflip
    this->last_flip = millis();
    this->isFilped = false;
  } else {
    this->flip->run(-255);
    this->last_flip = millis();
    this->isFilped = true;
  }
  return true;
}

void Arm::loop(long current) {
  if (this->last_flip && current - this->last_flip >= this->flip_time) {
    this->last_flip = 0;
    this->flip->run(0);
  }
  if (this->last_open_1 && current - this->last_open_1 >= this->open_time) {
    this->SmartServo->setPwmMove(this->handles[0], 0);
    this->last_open_1 = 0;
  }
  if (this->last_open_2 && current - this->last_open_2 >= this->open_time) {
    this->SmartServo->setPwmMove(this->handles[1], 0);
    this->last_open_2 = 0;
  }
}
