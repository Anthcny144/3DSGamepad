#pragma once
#include <3ds.h>
#include <string>

namespace Color {
    extern const std::string reset,
                             invalid,
                             valid,
                             disabled;
};

namespace Console {
    extern const u8 char_size;

    extern const u32 top_max_chars,
                     bot_max_chars;

    extern PrintConsole console_top, console_bottom;

    bool init();
    u8 print(bool top_screen, u8 line, u8 column, const std::string& text);
    void print_centered(bool top_screen, u8 line, const std::string& text);
    void print_error();
    void print_banner();
    void print_console_ip();
    void print_dest_ip();
    void print_port();
    void print_frequency();
    void print_ssid();
    void print_must_set_ip(bool show);
    void print_hold_to_stop(bool show);
    void print_pointless_on_emu();
    void print_buttons();
};
