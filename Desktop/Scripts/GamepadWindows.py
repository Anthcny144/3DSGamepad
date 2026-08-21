import vgamepad as vg
from .Gamepad import Gamepad, GamepadButton

class GamepadWindows(Gamepad):
    BUTTON_MAPPING: dict[GamepadButton, vg.XUSB_BUTTON] = {
        GamepadButton.A: vg.XUSB_BUTTON.XUSB_GAMEPAD_A,
        GamepadButton.B: vg.XUSB_BUTTON.XUSB_GAMEPAD_B,
        GamepadButton.X: vg.XUSB_BUTTON.XUSB_GAMEPAD_X,
        GamepadButton.Y: vg.XUSB_BUTTON.XUSB_GAMEPAD_Y,
        
        GamepadButton.L: vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER,
        GamepadButton.R: vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER,

        GamepadButton.START: vg.XUSB_BUTTON.XUSB_GAMEPAD_START,
        GamepadButton.SELECT: vg.XUSB_BUTTON.XUSB_GAMEPAD_BACK,

        GamepadButton.DPAD_UP: vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP,
        GamepadButton.DPAD_DOWN: vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN,
        GamepadButton.DPAD_LEFT: vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT,
        GamepadButton.DPAD_RIGHT: vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT,

        GamepadButton.THUMB_LEFT: vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB,
        GamepadButton.THUMB_RIGHT: vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB
    }

    def __init__(self):
        super().__init__()
        self.gamepad = vg.VX360Gamepad()

    def press_button(self, button: GamepadButton) -> None:
        self.gamepad.press_button(GamepadWindows.BUTTON_MAPPING[button])

    def release_button(self, button: GamepadButton) -> None:
        self.gamepad.release_button(GamepadWindows.BUTTON_MAPPING[button])

    def left_joystick(self, x: int, y: int) -> None:
        self.gamepad.left_joystick(x, y)

    def right_joystick(self, x: int, y: int) -> None:
        self.gamepad.right_joystick(x, y)

    def left_trigger(self, value: int) -> None:
        self.gamepad.left_trigger(value)

    def right_trigger(self, value: int) -> None:
        self.gamepad.right_trigger(value)

    def update(self) -> None:
        self.gamepad.update()