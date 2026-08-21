from enum import StrEnum
from .Config import KeyboardConfig
from .Gamepad import Gamepad
from .Input import Input
from .Keyboard import Keyboard
from .Utils import Utils

class ControllerType(StrEnum):
    GAMEPAD = "gamepad"
    KEYBOARD = "keyboard"

class InputManager:
    gamepad: Gamepad | None = None
    controller_type: ControllerType | None = None

    @staticmethod
    def init(controller_type_str: str, keyboard_config: KeyboardConfig) -> None:
        InputManager.controller_type = ControllerType(controller_type_str)

        # Create gamepad depending on OS
        if InputManager.controller_type == ControllerType.GAMEPAD:
            InputManager.gamepad = InputManager.get_os_gamepad()

        # Keyboard mode
        elif InputManager.controller_type == ControllerType.KEYBOARD:
            Keyboard.init(keyboard_config)

    @staticmethod
    def get_os_gamepad() -> Gamepad:
        if Utils.is_windows():
            from .GamepadWindows import GamepadWindows
            return GamepadWindows()

        else:
            from .GamepadLinux import GamepadLinux
            return GamepadLinux()

    @staticmethod
    def inject(state: Input):
        if InputManager.controller_type == ControllerType.GAMEPAD:
            assert InputManager.gamepad is not None
            InputManager.gamepad.apply_state(state)

        elif InputManager.controller_type == ControllerType.KEYBOARD:
            Keyboard.update(state)

    @staticmethod
    def reset() -> None:
        if InputManager.controller_type == ControllerType.GAMEPAD:
            assert InputManager.gamepad is not None
            InputManager.gamepad.disconnect()

        elif InputManager.controller_type == ControllerType.KEYBOARD:
            Keyboard.disconnect()
