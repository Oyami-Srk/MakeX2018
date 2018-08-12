#ifndef __BULLETFIRE_H__
#define __BULLETFIRE_H__

#include <MeDCMotor.h>
#include <MeEncoderMotor.h>
#include <MeEncoderNew.h>
#include <MeMegaPiProESCMotor.h>
#include <MeSmartServo.h>

/*
 * SmartServo: index-3 (untested)
 * angle: (untested)
 * MeMegaPiProESCMotor motor(3,4), 100% power
 * note: active isr_process_encoder funcs
 * Trigger motor: m11 (undex=3), power 50 1.2s
 */

class BulletFire {
public:
  BulletFire(MeSmartServo *ss);
  bool SwitchFire();
  bool SwitchAngle();
  bool Power();
  void loop(long current);

private:
  MeSmartServo *SmartServo;
  MeMegaPiProESCMotor *bl1;
  MeMegaPiProESCMotor *bl2;
  MeDCMotor *fire;
  bool fire_angle = false; // false->flat
  long last = 0;           // if not 0, delay it+1.2s
  long last_bl = 0;
  static const long fire_time = 1200;
  static const long delay_time = 2000;
  bool isOpen = false;
};

#endif
