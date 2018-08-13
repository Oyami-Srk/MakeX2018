#include "collector.h"

Collector::Collector(int index) { this->motor = new MeDCMotor(index); }

void Collector::Switch() {
  if (this->isRunning) {
    this->motor->run(0);
    this->isRunning = false;
  } else {
    this->motor->run(255);
    this->isRunning = true;
  }
}
