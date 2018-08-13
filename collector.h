#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include <MeDCMotor.h>
#include <MeEncoderMotor.h>
#include <MeEncoderNew.h>

// use m12(index =4)

class Collector {
public:
  Collector(int index = 4);
  void Switch();

private:
  MeDCMotor *motor;
  bool isRunning = false;
};

#endif
