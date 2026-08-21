#include <3ds.h>
#include "Button.hpp"
#include "Input.hpp"

Button::Button(const std::string& text, PrintConsole* console, u8 x, u8 y, u8 width_padding, u8 height_padding, bool shown, bool enabled, void (*click_callback)()) :
_text(text), _console(console), _x(x), _y(y), _width_padding(width_padding), _height_padding(height_padding), _shown(shown), _enabled(enabled), _click_callback(click_callback) {
    _calculate_size();
    _calculate_padding();
    _calculate_strings();
    _is_top_screen = _console->consoleWidth == 50;
    _should_redraw = false;
}

void Button::_calculate_size() {
    _width = 2 + _text.length() + _width_padding * 2;
    _height = 3 + _height_padding * 2;
}

void Button::_calculate_padding() {
    _padding_short = std::string(_width_padding, ' ');
    _padding_large = std::string(_width - 2, ' ');
}

void Button::_calculate_strings() {
    // Top & bottom framings
    _framing = "#";
    for (size_t i = 0; i < _width - 2; i++)
        _framing += '-';
    _framing += "#";

    // Padding line
    _padding_line = "|" + _padding_large + "|";

    // Text line
    _text_line = "|" + _padding_short + _text + _padding_short + "|";

    // Empty line
    _empty_line = std::string(_width, ' ');

}
bool Button::_is_clicked(u32 pressed, touchPosition& touch) {
    if (!Input::is_down(pressed, Keys::TOUCH))
        return false;

    u16 x = _x * Console::char_size,
        y = _y * Console::char_size,
        w = _width * Console::char_size,
        h = _height * Console::char_size;

    return !_is_top_screen && _enabled && (touch.px >= x && touch.px < x + w) && (touch.py >= y && touch.py < y + h);
}

void Button::print() {
    consoleSelect(_console);
    u8 x = _x + 1;
    u8 y = _y + 1;

    // Color
    Console::print(_is_top_screen, 0, 0, _enabled ? Color::reset : Color::disabled);

    y = Console::print(_is_top_screen, y, x, _shown ? _framing : _empty_line);

    for (size_t i = 0; i < _height_padding; i++)
        y = Console::print(_is_top_screen, y, x, _shown ? _padding_line : _empty_line);
    
    y = Console::print(_is_top_screen, y, x, _shown ? _text_line : _empty_line);

    for (size_t i = 0; i < _height_padding; i++)
        y = Console::print(_is_top_screen, y, x, _shown ? _padding_line : _empty_line);

    y = Console::print(_is_top_screen, y, x, _shown ? _framing : _empty_line);

    // Color
    Console::print(_is_top_screen, 0, 0, Color::reset);
}

void Button::run(u32 pressed, touchPosition& touch) {
    if (_should_redraw) {
        _should_redraw = false;
        print();
    }

    if (_click_callback == nullptr || !_shown)
        return;

    if (_is_clicked(pressed, touch))
        _click_callback();
}

void Button::set_visibility(bool shown) {
    _shown = shown;
    _should_redraw = true;
}

void Button::set_state(bool enabled) {
    _enabled = enabled;
    _should_redraw = true;
}
