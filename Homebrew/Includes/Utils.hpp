#pragma once
#include <3ds.h>
#include <string>

struct SafeStoiRes {
    int error;
    int val;
};

namespace Utils {
    SafeStoiRes safe_stoi(const std::string& text);
    size_t get_actual_str_length(const std::string& text);
    bool is_emu();
};

struct Clock {
    Clock();
    bool has_passed(u64 ms);
    void reset();
    u64 time;
};