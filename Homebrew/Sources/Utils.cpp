#include <3ds.h>
#include "Utils.hpp"

namespace Utils {
    SafeStoiRes safe_stoi(const std::string& text) {
        SafeStoiRes res;

        if (text.empty()) {
            res.error = 1;
            res.val = 0;
        }

        errno = 0;
        char* end_ptr = nullptr;

        long val = strtol(text.c_str(), &end_ptr, 10);

        if (end_ptr == text.c_str() || *end_ptr != '\0' || errno == ERANGE) {
            res.error = 2;
            res.val = 0;
        }

        res.error = 0;
        res.val = static_cast<int>(val);
        return res;
    }

    size_t get_actual_str_length(const std::string& text) {
        size_t length = 0;
        size_t i = 0;

        while (i < text.length()) {
            if (text[i] == '\x1b' && i + 1 < text.length() && text[i + 1] == '[') {
                i += 2; // Skip \x1b[

                while (i < text.length() && !isalpha(static_cast<unsigned char>(text[i])))
                    i++;

                if (i < text.length())
                    i++; // Skip the final char (H, m...)
            }
            else {
                length++;
                i++;
            }
        }

        return length;
    }

    bool is_emu() {
        s64 output = 0;
        svcGetSystemInfo(&output, 0x20000, 0);
        return output > 0;
    }
};

Clock::Clock() {
    reset();
}

bool Clock::has_passed(u64 ms) {
    u64 now = osGetTime();
    bool res = now - time >= ms;

    if (res)
        reset();

    return res;
}

void Clock::reset() {
    time = osGetTime();
}