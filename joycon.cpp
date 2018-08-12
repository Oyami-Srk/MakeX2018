#include "joycon.h"

Joycon::Joycon(MePS2 *ps2_control, Print *printer) {
    this->_control = ps2_control;
    memset(&this->_status, 0, sizeof(Joycon_Status));
}

int Joycon::append_invoke_func(joycon_invoke_func func) {
    if (this->invoke_count > MAX_INVOKE_LIST_COUNT)
        return 1;
    this->_invoke_list[invoke_count++] = func;
}

int Joycon::remove_invoke_func(int id, joycon_invoke_func func) {
    if (id == 0 && func == nullptr)
        return 1;
    if (!id) {
        for (int i = 0; i < this->invoke_count; i++)
            if (this->_invoke_list[i] == func) {
                id = i;
                break;
            }
    }
    if (!id)
        return 1;
    this->invoke_count--;
    if (id < 0 || id > this->invoke_count)
        return 1;
    for (int i = id; i < this->invoke_count; i++)
        this->_invoke_list[i] = this->_invoke_list[i + 1];
}

void Joycon::loop() {
    // Update _status
    this->_status.R.X = this->_control->MeAnalog(MeJOYSTICK_RX);
    this->_status.R.Y = this->_control->MeAnalog(MeJOYSTICK_RY);
    this->_status.L.X = this->_control->MeAnalog(MeJOYSTICK_LX);
    this->_status.L.Y = this->_control->MeAnalog(MeJOYSTICK_LX);
    this->_status.R1 = this->_control->ButtonPressed(MeJOYSTICK_R1);
    this->_status.UP = this->_control->ButtonPressed(MeJOYSTICK_UP);
    this->_status.DOWN = this->_control->ButtonPressed(MeJOYSTICK_DOWN);
    this->_status.LEFT = this->_control->ButtonPressed(MeJOYSTICK_LEFT);
    this->_status.RIGHT = this->_control->ButtonPressed(MeJOYSTICK_RIGHT);
    this->_status.KEY1 = this->_control->ButtonPressed(MeJOYSTICK_SQUARE);
    this->_status.KEY2 = this->_control->ButtonPressed(MeJOYSTICK_TRIANGLE);
    this->_status.KEY3 = this->_control->ButtonPressed(MeJOYSTICK_XSHAPED);
    this->_status.KEY4 = this->_control->ButtonPressed(MeJOYSTICK_ROUND);
    this->_status.KEY_R = this->_control->ButtonPressed(MeJOYSTICK_BUTTON_R);
    this->_status.KEY_L = this->_control->ButtonPressed(MeJOYSTICK_BUTTON_L);
    this->_status.MODE = this->_control->ButtonPressed(MeJOYSTICK_MODE);
    this->_status.SELECT = this->_control->ButtonPressed(MeJOYSTICK_SELECT);
    this->_status.START = this->_control->ButtonPressed(MeJOYSTICK_START);
    this->_status.R2 = this->_control->ButtonPressed(MeJOYSTICK_R2);
    this->_status.L1 = this->_control->ButtonPressed(MeJOYSTICK_L1);
    this->_status.L2 = this->_control->ButtonPressed(MeJOYSTICK_L2);

    // invoke func
    for (int i = 0; i < this->invoke_count; i++)
        this->_invoke_list[i](&this->_status);

    this->_control->loop();
}

void Joycon::begin(long BR) { this->_control->begin(BR); }