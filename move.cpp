#include "move.h"

Movement::Movement(int encoder_info[4][2]) {
  this->X1 = new _ENCODER_(_ENCODER_BASE_ADDR_ + encoder_info[0][0],
                           encoder_info[0][1]);
  this->X2 = new _ENCODER_(_ENCODER_BASE_ADDR_ + encoder_info[1][0],
                           encoder_info[1][1]);
  this->Y1 = new _ENCODER_(_ENCODER_BASE_ADDR_ + encoder_info[2][0],
                           encoder_info[2][1]);
  this->Y2 = new _ENCODER_(_ENCODER_BASE_ADDR_ + encoder_info[3][0],
                           encoder_info[3][1]);
}

void Movement::movement_raw(enum movement_raw_type type, int power, int speed) {
  switch (type) {
  case ystop:
    this->Y1->runSpeed(0);
    this->Y2->runSpeed(0);
    break;
  case xstop:
    this->X1->runSpeed(0);
    this->X2->runSpeed(0);
    break;
  case forward:

    this->Y1->runSpeed(speed);
    this->Y2->runSpeed(-speed);
    break;
  case backward:
    this->Y1->runSpeed(-speed);
    this->Y2->runSpeed(speed);
    break;
  case right:
    this->X1->runSpeed(speed);
    this->X2->runSpeed(-speed);
    break;
  case left:
    this->X1->runSpeed(-speed);
    this->X2->runSpeed(speed);
    break;
  case turn_right:
    this->X1->runSpeed(speed);
    this->X2->runSpeed(speed);
    this->Y1->runSpeed(speed);
    this->Y2->runSpeed(speed);
    break;
  case turn_left:
    this->X1->runSpeed(-speed);
    this->X2->runSpeed(-speed);
    this->Y1->runSpeed(-speed);
    this->Y2->runSpeed(-speed);
    break;
  }
}

int Movement::handle_joycon_y(Joycon_Status *status) {
  int y = status->R.Y;
  if (abs(y) < TRIGGER_MOVEMENT_JOYCON) {
    this->movement_raw(ystop);
  } else {
    Serial.print("[MOVEMENT] Y-asis: ");
    Serial.println(y);
    /*this->movement_raw(y < 0 ? backward : forward, 255,
                       this->SPEED_ * (abs(y) - TRIGGER_MOVEMENT_JOYCON) /
                           (255 - TRIGGER_MOVEMENT_JOYCON));*/
    this->movement_raw(y < 0 ? backward : forward, 255, 100);
  }
  return 0;
}

int Movement::handle_joycon_x(Joycon_Status *status) {
  int x = status->R.X;
  if (abs(x) < TRIGGER_MOVEMENT_JOYCON) {
    this->movement_raw(xstop);
  } else {
    Serial.print("[MOVEMENT] X-asis: ");
    Serial.println(x);
    /*this->movement_raw(x < 0 ? right : left, 255,
                       this->SPEED_ * (abs(x) - TRIGGER_MOVEMENT_JOYCON) /
                           (255 - TRIGGER_MOVEMENT_JOYCON));*/
    this->movement_raw(x < 0 ? right : left, 255, 100);
  }
  return 0;
}

int Movement::handle_joycon_k(Joycon_Status *status) {
  if ((!status->KEY2 && !status->KEY4) &&
      (abs(status->R.X) < TRIGGER_MOVEMENT_JOYCON) &&
      (abs(status->R.Y) < TRIGGER_MOVEMENT_JOYCON)) {
    this->movement_raw(xstop);
    this->movement_raw(ystop);
    return 0;
  }
  if (status->KEY2) {
    this->movement_raw(turn_right, 255, 80);
  } else if (status->KEY4) {
    this->movement_raw(turn_left, 255, 80);
  }
}

void Movement::begin() {
  this->X1->begin();
  this->X2->begin();
  this->Y1->begin();
  this->Y2->begin();
  this->Y1->setPWM(255);
  this->Y2->setPWM(255);
  this->X1->setPWM(255);
  this->X2->setPWM(255);
}