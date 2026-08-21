#include <3ds.h>
#include "Input.hpp"

void Input::scan() {
    hidScanInput();
}

u32 Input::get_buttons(bool get_pressed) {
    return get_pressed ? hidKeysDown() : hidKeysHeld();
}

circlePosition Input::get_cpad() {
    circlePosition cpad;
    hidCircleRead(&cpad);
    return cpad;
}

circlePosition Input::get_cstick() {
    circlePosition cstick;
    hidCstickRead(&cstick);
    return cstick;
}

touchPosition Input::get_touch() {
    touchPosition touch;
    hidTouchRead(&touch);
    return touch;
}

bool Input::is_down(u32 currently_held, u32 keys) {
    return (currently_held & keys) == keys;
}