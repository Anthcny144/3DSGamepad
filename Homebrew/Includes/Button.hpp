#pragma once
#include <3ds.h>
#include "Console.hpp"
#include <string>

class Button {
    std::string _text;
    PrintConsole* _console;
    u8 _x, _y, _width_padding, _height_padding;
    bool _shown, _enabled, _is_top_screen, _should_redraw;
    size_t _width, _height;
    std::string _padding_short, _padding_large, _framing, _padding_line, _text_line, _empty_line;
    void (*_click_callback)();

    void _calculate_size();
    void _calculate_padding();
    void _calculate_strings();
    bool _is_clicked(u32 pressed, touchPosition& touch);

    public:
    Button(const std::string& text, PrintConsole* console, u8 x, u8 y, u8 width_padding, u8 height_padding, bool shown, bool enabled, void (*click_callback)());
    void print();
    void run(u32 pressed, touchPosition& touch);
    void set_visibility(bool shown);
    void set_state(bool enabled);
};
