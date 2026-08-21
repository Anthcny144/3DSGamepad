import keyboard as kb
import mouse
import time
from enum import StrEnum
from .Config import KeyboardConfig
from .Input import Input, DSKey
from .Utils import Vector2D

class KeyboardClickMode(StrEnum):
    CLASSIC = "classic"
    MULTIPLE = "multiple"

class Keyboard:
    type KeyboardMapping = dict[DSKey, str]
    type KeyState = dict[str, dict[str, float]]

    REPEAT_DELAY = 0.4
    REPEAT_INTERVAL = 0.02

    key_states: KeyState = {}
    keyboard_mapping: KeyboardMapping = {}
    keyboard_click_mapping: KeyboardMapping = {
        DSKey.CPAD_LEFT: "left",
        DSKey.CPAD_RIGHT: "right",
        DSKey.CPAD_UP: "middle",
        DSKey.CSTICK_LEFT: "x",
        DSKey.CSTICK_RIGHT: "x2"
    }
    click_mode: KeyboardClickMode | None = None
    held_clicks = 0
    click_release: Vector2D | None = None
    first_hold = True
    lock_click = False

    @staticmethod
    def init(keyboard_config: KeyboardConfig):
        Keyboard.click_mode = KeyboardClickMode(keyboard_config.CLICK_MODE)

        for ds_key_str, keyboard_key in keyboard_config:
            ds_key_int = DSKey.str_to_int(ds_key_str)

            if ds_key_int is not None:
                Keyboard.keyboard_mapping[ds_key_int] = keyboard_key

    @staticmethod
    def update(state: Input) -> None:
        # Prevent the first touch (when clicking the "Connect" button) to register
        if Keyboard.first_hold and not state.is_touch():
            Keyboard.first_hold = False

        Keyboard.update_keys(state)
        Keyboard.update_touch(state)

        if Keyboard.click_mode == KeyboardClickMode.MULTIPLE:
            Keyboard.update_clicks_multiple(state)

    @staticmethod
    def update_keys(state: Input) -> None:
        for key_bit, key in Keyboard.keyboard_mapping.items():
            if not key:
                continue

            now = time.perf_counter()

            if state.buttons & key_bit:
                if key not in Keyboard.key_states:
                    kb.press(key)
                    Keyboard.key_states[key] = {"pressed": now, "repeat": now}

                else:
                    key_state = Keyboard.key_states[key]
                    time_held = now - key_state["pressed"]

                    if time_held >= Keyboard.REPEAT_DELAY:
                        time_since_repeat = now - key_state["repeat"]
                        if time_since_repeat >= Keyboard.REPEAT_INTERVAL:
                            kb.press(key)
                            key_state["repeat"] = now
            else:
                if key in Keyboard.key_states:
                    kb.release(key)
                    del Keyboard.key_states[key]

    @staticmethod
    def update_touch(state: Input) -> None:
        if Keyboard.first_hold:
            return

        if state.is_touch():
            mouse.move(state.touch.x, state.touch.y)

            if Keyboard.click_mode == KeyboardClickMode.CLASSIC and not Keyboard.lock_click:
                mouse.press("left")
                Keyboard.lock_click = True

        elif Keyboard.lock_click:
            mouse.release("left")
            Keyboard.lock_click = False

    @staticmethod
    def update_clicks_multiple(state: Input) -> None:
        for key_bit, key in Keyboard.keyboard_click_mapping.items():
            if state.buttons & key_bit and not Keyboard.held_clicks & key_bit:
                mouse.press(key)
                Keyboard.held_clicks |= key_bit
            elif not state.buttons & key_bit and Keyboard.held_clicks & key_bit:
                mouse.release(key)
                Keyboard.held_clicks ^= key_bit

    @staticmethod
    def disconnect() -> None:
        for key in Keyboard.key_states.keys():
            kb.release(key)

        # TODO release correct mouse click
        Keyboard.first_hold = True
        Keyboard.lock_click = False
        Keyboard.key_states.clear()