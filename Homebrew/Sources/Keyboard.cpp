#include <3ds.h>
#include "Input.hpp"
#include "Keyboard.hpp"
#include "Network.hpp"

namespace Keyboard {
    SwkbdState kb;
    char written_dest_ip[16], written_frequency[4];

    SwkbdCallbackResult check_ip_callback(void* user, const char** internal_popup, const char* input, size_t input_length) {
        if (input_length == 0)
            return SWKBD_CALLBACK_OK;

        bool valid = Network::is_valid_ip(std::string(input));

        if (!valid) {
            *internal_popup = "You must enter a valid IPv4.";
            return SWKBD_CALLBACK_CONTINUE;
        }

        return SWKBD_CALLBACK_OK;
    }

    SwkbdCallbackResult check_hz_callback(void* user, const char** internal_popup, const char* input, size_t input_length) {
        if (input_length == 0)
            return SWKBD_CALLBACK_OK;

        bool valid = input_length > 0 && input[0] != '0';

        if (!valid) {
            *internal_popup = "You must enter a number\nhigher than 0.";
            return SWKBD_CALLBACK_CONTINUE;
        }

        return SWKBD_CALLBACK_OK;
    }

    KeyboardAns open_set_dest_ip() {
        swkbdInit(&kb, SWKBD_TYPE_NUMPAD, 2, 15);
        swkbdSetHintText(&kb, "Enter the destination IP");
        swkbdSetInitialText(&kb, written_dest_ip);
        swkbdSetNumpadKeys(&kb, 0, L'.');
        swkbdSetValidation(&kb, SWKBD_NOTEMPTY_NOTBLANK, SWKBD_FILTER_CALLBACK, 12);
        swkbdSetFeatures(&kb, SWKBD_FIXED_WIDTH);
        swkbdSetFilterCallback(&kb, check_ip_callback, nullptr);
        return Keyboard::answer(written_dest_ip, sizeof(written_dest_ip));
    }

    KeyboardAns open_set_frequency() {
        swkbdInit(&kb, SWKBD_TYPE_NUMPAD, 2, 3);
        swkbdSetHintText(&kb, "Enter the amount of packet to send every second");
        swkbdSetInitialText(&kb, written_frequency);
        swkbdSetValidation(&kb, SWKBD_NOTEMPTY_NOTBLANK, SWKBD_FILTER_CALLBACK, 3);
        swkbdSetFeatures(&kb, SWKBD_FIXED_WIDTH);
        swkbdSetFilterCallback(&kb, check_hz_callback, nullptr);
        return Keyboard::answer(written_frequency, sizeof(written_frequency));
    }

    KeyboardAns answer(char* written, size_t length) {
        std::string backup(written);

        KeyboardAns ans;
        ans.button = swkbdInputText(&kb, written, length);

        if (ans.button != SWKBD_BUTTON_CONFIRM) {
            strncpy(written, backup.c_str(), length - 1);
            written[length - 1] = '\0';
            ans.text = backup;
        }
        else
            ans.text = std::string(written);

        return ans;
    }
};
