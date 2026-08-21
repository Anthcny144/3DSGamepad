#pragma once
#include <3ds.h>
#include <string>

struct KeyboardAns {
    SwkbdButton button;
    std::string text;
};

namespace Keyboard {
    extern SwkbdState kb;
    extern char written_dest_ip[16], written_frequency[4];

    SwkbdCallbackResult check_ip_callback(void* user, const char** internal_popup, const char* input, size_t input_length);
    SwkbdCallbackResult check_hz_callback(void* user, const char** internal_popup, const char* input, size_t input_length);
    KeyboardAns open_set_dest_ip();
    KeyboardAns open_set_frequency();
    KeyboardAns answer(char* written, size_t length);
};
