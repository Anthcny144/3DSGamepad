import uinput
from .Gamepad import Gamepad, GamepadButton

class GamepadLinux(Gamepad):
    type LinuxBtn = tuple[int, int]

    EVENTS = (
        uinput.BTN_A,
        uinput.BTN_B,
        uinput.BTN_X,
        uinput.BTN_Y,
        uinput.BTN_TL,
        uinput.BTN_TR,
        uinput.BTN_START,
        uinput.BTN_SELECT,
        uinput.BTN_THUMBL,
        uinput.BTN_THUMBR,
        uinput.ABS_Z + (-32768, 32767, 0, 0),
        uinput.ABS_RZ + (-32768, 32767, 0, 0),
    )

    BUTTON_MAPPING: dict[GamepadButton, LinuxBtn] = {
        GamepadButton.A: uinput.BTN_A,
        GamepadButton.B: uinput.BTN_B,
        GamepadButton.X: uinput.BTN_X,
        GamepadButton.Y: uinput.BTN_Y,
        
        GamepadButton.L:  uinput.BTN_TL,
        GamepadButton.R:  uinput.BTN_TR,
        GamepadButton.ZL: uinput.ABS_Z,
        GamepadButton.ZR: uinput.ABS_RZ,

        GamepadButton.START: uinput.BTN_START,
        GamepadButton.SELECT: uinput.BTN_SELECT,

        GamepadButton.DPAD_UP: uinput.BTN_DPAD_UP,
        GamepadButton.DPAD_DOWN: uinput.BTN_DPAD_DOWN,
        GamepadButton.DPAD_LEFT: uinput.BTN_DPAD_LEFT,
        GamepadButton.DPAD_RIGHT: uinput.BTN_DPAD_RIGHT,

        GamepadButton.THUMB_LEFT: uinput.BTN_THUMBL,
        GamepadButton.THUMB_RIGHT: uinput.BTN_THUMBR
    }

    def __init__(self):
        self.device = uinput.Device(GamepadLinux.EVENTS)

    def press_button(self, button: GamepadButton) -> None:
        self.device.emit(GamepadLinux.BUTTON_MAPPING[button], 1, False)

    def release_button(self, button: GamepadButton) -> None:
        self.device.emit(GamepadLinux.BUTTON_MAPPING[button], 0, False)

    def left_joystick(self, x: int, y: int) -> None:
        self.device.emit(uinput.ABS_X, x, False)
        self.device.emit(uinput.ABS_Y, y, False)

    def right_joystick(self, x: int, y: int) -> None:
        self.device.emit(uinput.ABS_RX, x, False)
        self.device.emit(uinput.ABS_RY, y, False)

    def left_trigger(self, value: int) -> None:
        self.device.emit(GamepadLinux.BUTTON_MAPPING[GamepadButton.THUMB_LEFT], value, False)

    def right_trigger(self, value: int) -> None:
        self.device.emit(GamepadLinux.BUTTON_MAPPING[GamepadButton.THUMB_RIGHT], value, False)

    def update(self) -> None:
        self.device.syn()