#include <3ds.h>
#include "App.hpp"
#include "Button.hpp"
#include "Console.hpp"
#include "Keyboard.hpp"
#include "Network.hpp"
#include "Utils.hpp"

namespace Color {
    const std::string reset = "\x1b[0m",
                      invalid = "\x1b[41m",
                      valid = "\x1b[32m",
                      disabled = "\x1b[2m\x1b[39m";
};

namespace Console {
    const u8 char_size = 8;

    const u32 top_max_chars = 50,
              bot_max_chars = 40;

    PrintConsole console_top, console_bottom;

    bool init() {
        if (!consoleInit(GFX_TOP, &console_top) || !consoleInit(GFX_BOTTOM, &console_bottom))
            return false;

        return true;
    }

    u8 print(bool top_screen, u8 line, u8 column, const std::string& text) {
        consoleSelect(top_screen ? &console_top : &console_bottom);
        std::string actual = "\x1b[" + std::to_string(line) + ";" + std::to_string(column) + "H" + text;
        printf("%s", actual.c_str());
        return line + 1;
    }

    void print_centered(bool top_screen, u8 line, const std::string& text) {
        int column = ((top_screen ? top_max_chars : bot_max_chars) - Utils::get_actual_str_length(text)) / 2 + 1;
        print(top_screen, line, column, text);
    }

    void print_error() {
        consoleSelect(&console_top);
        consoleClear();
        consoleSelect(&console_bottom);
        consoleClear();
        print_centered(true, 10, Color::invalid + Network::error + Color::reset);
    }

    void print_banner() {
        static Button btn(
            "3DS Gamepad",
            &console_top,
            17, 3, 2, 1,
            true, true,
            nullptr
        );

        btn.print();
    }

    void print_console_ip() {
        Network::get_console_ip();
        bool valid = Network::is_valid_ip(Network::ds_ip);
        print(true, 11, 15, "Console IP: " + (valid ? Color::valid : Color::invalid) + Network::ds_ip + Color::reset + "      ");
    }

    void print_dest_ip() {
        print(true, 13, 15, "Dest IP: " + (Network::valid_dest_ip ? Color::valid : Color::invalid) + Network::dest_ip + Color::reset + "        ");
    }

    void print_port() {
        print(true, 15, 15, "Port: " + Color::valid + std::to_string(Network::port) + Color::reset);
    }

    void print_frequency() {
        print(true, 17, 15, "Packets per second: " + Color::valid + std::to_string(Network::frequency) + Color::reset + "    ");
    }

    void print_ssid() {
        char ssid[32];
        acInit();
        ACU_GetSSID(ssid);
        acExit();
        print(true, 19, 15, "SSID: " + Color::valid + std::string(ssid) + Color::reset + "                 ");
    }

    void print_must_set_ip(bool show) {
        std::string text = Color::invalid + "You must set the destination IP" + Color::reset;
        print(true, 23, 12, !show ? std::string(Utils::get_actual_str_length(text), ' ') : text);
    }

    void print_hold_to_stop(bool show) {
        static const std::string plus = Color::reset + " + " + Color::valid;
        std::string text = "Hold " + Color::valid + "SELECT" + plus + "START" + plus + "X" + plus + "Y" + Color::reset + " to stop";
        print_centered(true, 23, !show ? std::string(Utils::get_actual_str_length(text), ' ') : text);
    }

    void print_pointless_on_emu() {
        print_centered(true, 29, Color::invalid + "Using this app on emulator is pointless" + Color::reset);
        print_centered(true, 30, Color::invalid + "and may not work" + Color::reset);
    }

    void print_buttons() {
        App::btn_set_dest_ip.print();
        App::btn_frequency.print();
        App::btn_connect.print();
    }
};
