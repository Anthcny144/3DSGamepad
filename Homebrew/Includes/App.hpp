#pragma once
#include <3ds.h>
#include "Button.hpp"

namespace App {
    extern bool errored, sending;
    extern u64 sleep_ns;

    extern Button btn_set_dest_ip,
                  btn_frequency,
                  btn_connect;

    int main();
    bool init();
    int exit();
    void run();
    void run_buttons(u32 pressed, touchPosition& touch);
    void set_buttons_visibility(bool shown);
    bool check_stop();
};
