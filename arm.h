#ifndef __ARM_H__
#define __ARM_H__

#include <MeDCMotor.h>
#include <MeEncoderMotor.h>
#include <MeEncoderNew.h>
#include <MeSmartServo.h>

/*
 * SmartServo: index-1: 50, index-2: -50 (rate not angle)
 * delay: 0.6s
 * Lift motor: m9 (index=1)
 * Flip motor: m10 (index=2)
 * note:
 * if isFliped = true and isLift = true, switchlift will do half ( for the
 * handle above the ground ) else if isFliped = true and isLift = true, also
 */
class Arm {
public:
  Arm(MeSmartServo *ss, int handles[2], int lift = 1, int flip = 2);
  bool SwitchHand(int id = 0); // id in handles, -1 means all
  bool SwitchFlip();
  bool SwitchLift(bool upward);
  void loop(long current);

private:
  MeSmartServo *SmartServo;
  int handles[2];
  MeDCMotor *flip;
  MeDCMotor *lift;
  bool isFilped = true; // true -> fliped (angle 90)
  bool isOpen_1 = false;
  bool isOpen_2 = false;
  long last_lift = 0;
  long last_flip = 0;
  long last_open_1 = 0;
  long last_open_2 = 0;

  static const long lift_time = 50;
  static const long flip_time = 1600;
  static const long open_time = 600;
};

#endif
