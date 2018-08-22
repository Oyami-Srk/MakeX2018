#include "collector.h"

Collector::Collector(int index) { this->motor = new MeDCMotor(index); }

void Collector::Switch() {
  if (this->_lock_)
    return;
  if (this->isRunning) {
    this->motor->run(0);
    this->isRunning = false;
  } else {
    this->motor->run(255);
    this->isRunning = true;
  }
  this->_lock_ = millis();
}

void Collector::UnSwitch() {
  if (this->_lock2_)
    return;
  if (this->isRunning) {
    this->motor->run(0);
    this->isRunning = false;
  } else {
    this->motor->run(-255);
    this->isRunning = true;
  }
  this->_lock2_ = millis();
}

void Collector::loop(long current) {
  if (this->_lock_ && current - this->_lock_ >= this->lock_time)
    this->_lock_ = 0;
  if (this->_lock2_ && current - this->_lock_ >= this->lock_time)
    this->_lock2_ = 0;
}