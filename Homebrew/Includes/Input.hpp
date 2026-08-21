#pragma once
#include <3ds.h>

enum Keys {
    A = BIT(0),
    B = BIT(1),
    X = BIT(10),
    Y = BIT(11),

    L = BIT(9),
    R = BIT(8),
    ZL = BIT(14),
    ZR = BIT(15),

    START = BIT(3),
    SELECT = BIT(2),

    UP = BIT(6),
    DOWN = BIT(7),
    LEFT = BIT(5),
    RIGHT = BIT(4),

    TOUCH = BIT(20)
};

struct Input {
    static void scan();
    static u32 get_buttons(bool get_pressed);
    static circlePosition get_cpad();
    static circlePosition get_cstick();
    static touchPosition get_touch();

    u32 buttons;
    circlePosition cpad;
    circlePosition cstick;
    touchPosition touch;

    static bool is_down(u32 currently_held, u32 keys);
};