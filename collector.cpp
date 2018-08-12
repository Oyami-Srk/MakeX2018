#include "collector.h"

Collector::Collector() { this->motor = new MeDCMotor(4); }

void Collector::Switch() {
  if (this->isRunning) {
    this->motor->run(0);
    this->isRunning = false;
  } else {
    this->motor->run(255);
    this->isRunning = true;
  }
}
