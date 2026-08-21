#include <3ds.h>
#include "App.hpp"
#include "Console.hpp"
#include "Input.hpp"
#include "Keyboard.hpp"
#include "Network.hpp"
#include "Settings.hpp"
#include "Utils.hpp"

namespace App {
    bool errored = false, sending = false;
    u64 sleep_ns = 1000000000LL / 60;

    Button btn_set_dest_ip = Button(
        "Set destination IP",
        &Console::console_bottom,
        9, 3, 1, 2,
        true, true,
        []() {
            KeyboardAns ans = Keyboard::open_set_dest_ip();
            if (ans.button != SWKBD_BUTTON_CONFIRM)
                return;

            Network::dest_ip = ans.text;
            Settings::save();

            btn_connect.set_state(true);
            Network::valid_dest_ip = true;
            Network::set_dest();
            Console::print_dest_ip();
            Console::print_must_set_ip(false);
        }
    );

    Button btn_frequency = Button(
        "Set frequency ",
        &Console::console_bottom,
        9, 12, 3, 2,
        true, true,
        []() {
            KeyboardAns ans = Keyboard::open_set_frequency();
            if (ans.button != SWKBD_BUTTON_CONFIRM)
                return;

            SafeStoiRes frequency_stoi = Utils::safe_stoi(ans.text);
            if (frequency_stoi.error != 0)
                return;

            Network::frequency = frequency_stoi.val;
            sleep_ns = 1000000000LL / Network::frequency;
            Console::print_frequency();
            Settings::save();
        }
    );

    Button btn_connect = Button(
        "Connect ",
        &Console::console_bottom,
        9, 21, 6, 2,
        true, false,
        []() {
            sending = true;
            Console::print_hold_to_stop(true);
            App::set_buttons_visibility(false);
        }
    );

    int main() {
        if (!init())
            return EXIT_FAILURE;

        while (aptMainLoop())
            run();

        return exit();
    }
    
    bool init() {
        gfxInitDefault();
    
        if (!Console::init())
            return false;

        Settings::load();
        Network::init();

        errored = !Network::error.empty();
        if (errored) {
            Console::print_error();
            return true;
        }

        Console::print_banner();
        Console::print_console_ip();
        Console::print_dest_ip();
        Console::print_port();
        Console::print_frequency();

        if (Utils::is_emu())
            Console::print_pointless_on_emu();
        else
            Console::print_ssid();

        if (!Network::valid_dest_ip)
            Console::print_must_set_ip(true);
        else
            btn_connect.set_state(true);

        Console::print_buttons();

        return true;
    }

    int exit() {
        Network::exit();
        acExit();
        gfxExit();
        return EXIT_SUCCESS;
    }

    void run() {
        if (errored) {
            gfxFlushBuffers();
            gfxSwapBuffers();
            svcSleepThread(sleep_ns);
            return;
        }

        static Clock clock = Clock();

        Input::scan();
        u32 buttons = Input::get_buttons(false);
        u32 pressed = Input::get_buttons(true);
        circlePosition cpad = Input::get_cpad();
        circlePosition cstick = Input::get_cstick();
        touchPosition touch = Input::get_touch();

        run_buttons(pressed & buttons, touch);
        Utils::is_emu();

        if (check_stop()) {
            sending = false;
            Console::print_hold_to_stop(false);
            App::set_buttons_visibility(true);
            Network::packet_id = 0;
        }

        if (sending) {
            Input input;
            input.buttons = buttons;
            input.cpad = cpad;
            input.cstick = cstick;
            input.touch = touch;

            Packet packet;
            packet.input = input;
            packet.frequency = Network::frequency;
            packet.packet_id = ++Network::packet_id;

            Network::send(packet);
        }

        // Update some info every 2 sec
        if (clock.has_passed(2000)) {
            Console::print_console_ip();
    
            if (!Utils::is_emu())
                Console::print_ssid();
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        svcSleepThread(sleep_ns);
    }

    void run_buttons(u32 pressed, touchPosition& touch) {
        btn_set_dest_ip.run(pressed, touch);
        btn_frequency.run(pressed, touch);
        btn_connect.run(pressed, touch);
    }
    
    void set_buttons_visibility(bool shown) {
        btn_set_dest_ip.set_visibility(shown);
        btn_frequency.set_visibility(shown);
        btn_connect.set_visibility(shown);
    }

    bool check_stop() {
        return sending && Input::is_down(Input::get_buttons(false), Keys::START | Keys::SELECT | Keys::X | Keys::Y);
    }
};
