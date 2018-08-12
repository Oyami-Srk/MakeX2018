#ifndef __MOVE_H__
#define __MOVE_H__

// Extra
#include "joycon.h"
#include <MeEncoderNew.h>

#define _ENCODER_ MeEncoderNew
#define _ENCODER_BASE_ADDR_ 0x09

enum movement_raw_type { xstop = 0, ystop, forward, backward, right, left };

class Movement {
public:
  Movement(int encoder_info[4][2]);
  void begin();
  int handle_joycon_x(Joycon_Status *status);
  int handle_joycon_y(Joycon_Status *status);

  void movement_raw(enum movement_raw_type type, int power = 255,
                    int speed = 180);

private:
  _ENCODER_ *X1 = nullptr;
  _ENCODER_ *X2 = nullptr;
  _ENCODER_ *Y1 = nullptr;
  _ENCODER_ *Y2 = nullptr;
};

#endif