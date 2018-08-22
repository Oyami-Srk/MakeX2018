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
  void UnSwitch();
  void loop(long current);

private:
  MeDCMotor *motor;
  bool isRunning = false;
  long _lock_ = 0;
  long _lock2_ = 0;
  long lock_time = 500;
};

#endif
