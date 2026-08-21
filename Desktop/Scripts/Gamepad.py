from abc import ABC, abstractmethod
from enum import Enum, auto
from .Input import Input, DSKey

class GamepadButton(Enum):
    A = auto()
    B = auto()
    X = auto()
    Y = auto()
    
    L = auto()
    R = auto()
    ZL = auto()
    ZR = auto()

    START = auto()
    SELECT = auto()

    DPAD_UP = auto()
    DPAD_DOWN = auto()
    DPAD_LEFT = auto()
    DPAD_RIGHT = auto()

    THUMB_LEFT = auto()
    THUMB_RIGHT = auto()

DS_MAPPING: dict[DSKey, GamepadButton] = {
    DSKey.A: GamepadButton.A,
    DSKey.B: GamepadButton.B,
    DSKey.X: GamepadButton.X,
    DSKey.Y: GamepadButton.Y,

    DSKey.L: GamepadButton.L,
    DSKey.R: GamepadButton.R,

    DSKey.START: GamepadButton.START,
    DSKey.SELECT: GamepadButton.SELECT,

    DSKey.DPAD_UP: GamepadButton.DPAD_UP,
    DSKey.DPAD_DOWN: GamepadButton.DPAD_DOWN,
    DSKey.DPAD_LEFT: GamepadButton.DPAD_LEFT,
    DSKey.DPAD_RIGHT: GamepadButton.DPAD_RIGHT,
}

class Gamepad(ABC):
    @abstractmethod
    def __init__(self) -> None:
        self.thumb_press: GamepadButton | None = None

    @abstractmethod
    def press_button(self, button: GamepadButton) -> None: ...

    @abstractmethod
    def release_button(self, button: GamepadButton) -> None: ...

    @abstractmethod
    def left_joystick(self, x: int, y: int) -> None: ...

    @abstractmethod
    def right_joystick(self, x: int, y: int) -> None: ...

    @abstractmethod
    def left_trigger(self, value: int) -> None: ...
    
    @abstractmethod
    def right_trigger(self, value: int) -> None: ...

    @abstractmethod
    def update(self) -> None: ...

    def apply_state(self, state: Input) -> None:
        # Buttons
        for key_bit, gamepad_button in DS_MAPPING.items():
            if state.buttons & key_bit:
                self.press_button(gamepad_button)
            else:
                self.release_button(gamepad_button)

        # ZL & ZR
        self.left_trigger(value=255 if (state.buttons & DSKey.ZL) else 0)
        self.right_trigger(value=255 if (state.buttons & DSKey.ZR) else 0)

        # C-pad
        self.left_joystick(state.cpad.x, state.cpad.y)

        # C-stick
        self.right_joystick(state.cstick.x, state.cstick.y)

        # Touch
        if state.is_touch() and self.thumb_press is None:
            is_left = state.touch_unscaled.x < Input.DS_TOUCH_SCREEN_WIDTH / 2
            self.thumb_press = GamepadButton.THUMB_LEFT if is_left else GamepadButton.THUMB_RIGHT
            self.press_button(self.thumb_press)
        elif not state.is_touch() and self.thumb_press is not None:
            self.release_button(self.thumb_press)
            self.thumb_press = None

        self.update()

    def disconnect(self) -> None:
        for _, gamepad_button in DS_MAPPING.items():
            self.release_button(gamepad_button)

        self.update()