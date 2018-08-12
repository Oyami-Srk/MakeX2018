#include "bullet_fire.h"

BulletFire::BulletFire(MeSmartServo *ss) {
  this->fire = new MeDCMotor(3);
  this->SmartServo = ss;
  this->SmartServo->setInitAngle(3);
  this->bl1 = new MeMegaPiProESCMotor(3);
  this->bl2 = new MeMegaPiProESCMotor(4);
  this->bl1->init();
  this->bl2->init();
}

bool BulletFire::SwitchFire() {
  if (this->last)
    return false;
  this->fire->run(50);
  this->last = millis();
  return true;
}

bool BulletFire::SwitchAngle() {
  if (!this->fire_angle) {
    this->SmartServo->moveTo(3,  // index
                             90, // angle untested
                             30  // speed rpm
    );
    this->fire_angle = true;
  } else {
    this->SmartServo->setInitAngle(3);
    this->fire_angle = false;
  }
  return true;
}

bool BulletFire::Power() {
  if (this->last_bl)
    return false;
  int power = 100;
  if (isOpen)
    power = 0;
  this->bl1->run(power);
  this->bl2->run(power);
  this->last_bl = millis();
  this->isOpen = !this->isOpen;
  return true;
}

void BulletFire::loop(long current) {
  if (this->last && current - this->last >= this->fire_time) {
    this->fire->run(0);
    this->last = 0;
  }
  if (this->last_bl && current - this->last_bl >= this->delay_time) {
    this->last_bl = 0;
  }
}
