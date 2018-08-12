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
    this->Y1->setPWM(0);
    this->Y2->setPWM(0);
    this->Y1->runSpeed(0);
    this->Y2->runSpeed(0);
    break;
  case xstop:
    this->X1->setPWM(0);
    this->X2->setPWM(0);
    this->X1->runSpeed(0);
    this->X2->runSpeed(0);
    break;
  case forward:
    this->Y1->setPWM(power);
    this->Y2->setPWM(power);
    this->Y1->runSpeed(speed);
    this->Y2->runSpeed(-speed);
    break;
  case backward:
    this->Y1->setPWM(power);
    this->Y2->setPWM(power);
    this->Y1->runSpeed(-speed);
    this->Y2->runSpeed(speed);
    break;
  case right:
    this->X1->setPWM(power);
    this->X2->setPWM(power);
    this->X1->runSpeed(speed);
    this->X2->runSpeed(-speed);
    break;
  case left:
    this->X1->setPWM(power);
    this->X2->setPWM(power);
    this->X1->runSpeed(-speed);
    this->X2->runSpeed(speed);
    break;
  }
}

int Movement::handle_joycon_y(Joycon_Status *status) {
  int y = status->R.Y;
  if (y > -TRIGGER_MOVEMENT_JOYCON && y < TRIGGER_MOVEMENT_JOYCON) {
    this->movement_raw(ystop);
  } else {
    this->movement_raw(y < 0 ? backward : forward, 255,
                       5000 * (abs(y) - TRIGGER_MOVEMENT_JOYCON) /
                           (255 - TRIGGER_MOVEMENT_JOYCON));
  }
  return 0;
}

int Movement::handle_joycon_x(Joycon_Status *status) {
  int x = status->R.X;
  if (x > -TRIGGER_MOVEMENT_JOYCON && x < TRIGGER_MOVEMENT_JOYCON) {
    this->movement_raw(xstop);
  } else {
    this->movement_raw(x < 0 ? right : left, 255,
                       5000 * (abs(x) - TRIGGER_MOVEMENT_JOYCON) /
                           (255 - TRIGGER_MOVEMENT_JOYCON));
  }
  return 0;
}

void Movement::begin() {
  this->X1->begin();
  this->X2->begin();
  this->Y1->begin();
  this->Y2->begin();
}