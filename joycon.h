#ifndef __JOYCON_H__
#define __JOYCON_H__

#include <MePS2.h>

// constant value
#define MAX_INVOKE_LIST_COUNT 4
#define TRIGGER_MOVEMENT_JOYCON 80

typedef struct {
  bool UP, DOWN, LEFT, RIGHT;
  bool KEY1, KEY2, KEY3, KEY4;
  bool KEY_R, KEY_L, MODE, SELECT, START;
  bool R1, R2, L1, L2;
  struct {
    int X, Y;
  } R, L;
} Joycon_Status;

typedef int (*joycon_invoke_func)(Joycon_Status *);

class Joycon {
public:
  Joycon(MePS2 *ps2_control, Print *printer = nullptr);

  void begin(long BR = 115200);

  int append_invoke_func(joycon_invoke_func func);

  int remove_invoke_func(int id = -1, joycon_invoke_func func = nullptr);

  void loop();

  MePS2 *_control;

private:
  joycon_invoke_func _invoke_list[MAX_INVOKE_LIST_COUNT];

  Joycon_Status _status;

  int invoke_count = 0;
};

#endif
