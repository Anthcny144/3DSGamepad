import yaml
from pydantic import BaseModel, ValidationError
from typing import Literal, ClassVar

type Keybind = Literal["", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
                       "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "numlock",
                       "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
                       "enter", "escape", "space", "delete", "backspace", "tab", "capslock",
                       "shift", "left shift", "right shift",
                       "control", "left control", "right control",
                       "alt", "alt gr",
                       "up_arrow", "down_arrow", "left_arrow", "right_arrow"]

class KeyboardConfig(BaseModel):
    A: Keybind
    B: Keybind
    X: Keybind
    Y: Keybind

    L: Keybind
    R: Keybind
    ZL: Keybind
    ZR: Keybind

    START: Keybind
    SELECT: Keybind

    DPAD_UP: Keybind
    DPAD_DOWN: Keybind
    DPAD_LEFT: Keybind
    DPAD_RIGHT: Keybind

    TOUCH_SENSITIVITY: float
    CLICK_MODE: Literal["classic", "multiple"]
    LINUX_SCREEN_SCALE: float

class Config(BaseModel):
    DEFAULT_PATH: ClassVar = "./config.yaml"

    CONTROLLER_TYPE: Literal["keyboard", "gamepad"]
    EXIT: Keybind
    TIMEOUT: float
    KEYBOARD_CONFIG: KeyboardConfig

    @staticmethod
    def load(path: str) -> "Config | None":
        try:
            with open(path) as file:
                data = yaml.safe_load(file)

            return Config(**data)
        except ValidationError as ex:
            print(f"Invalid config file: {ex}")
        except FileNotFoundError as ex:
            print(f"Config file not found")

        return None