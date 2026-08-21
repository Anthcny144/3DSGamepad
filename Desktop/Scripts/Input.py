import screeninfo
from dataclasses import dataclass
from enum import IntEnum
from .Network import Network
from .Utils import Utils, Vector2D, Rect

class DSKey(IntEnum):
    A = 1 << 0
    B = 1 << 1
    X = 1 << 10
    Y = 1 << 11

    L = 1 << 9
    R = 1 << 8
    ZL = 1 << 14
    ZR = 1 << 15

    START = 1 << 3
    SELECT = 1 << 2

    DPAD_UP = 1 << 6
    DPAD_DOWN = 1 << 7
    DPAD_LEFT = 1 << 5
    DPAD_RIGHT = 1 << 4

    CPAD_UP = 1 << 30
    CPAD_DOWN = 1 << 31
    CPAD_LEFT = 1 << 29
    CPAD_RIGHT = 1 << 28

    CSTICK_UP = 1 << 26
    CSTICK_DOWN = 1 << 27
    CSTICK_LEFT = 1 << 25
    CSTICK_RIGHT = 1 << 24

    TOUCH = 1 << 20

    @staticmethod
    def str_to_int(key_str: str) -> "DSKey | None":
        mapping = {
            "A": DSKey.A,
            "B": DSKey.B,
            "X": DSKey.X,
            "Y": DSKey.Y,

            "L": DSKey.L,
            "R": DSKey.R,
            "ZL": DSKey.ZL,
            "ZR": DSKey.ZR,

            "START": DSKey.START,
            "SELECT": DSKey.SELECT,

            "DPAD_UP": DSKey.DPAD_UP,
            "DPAD_DOWN": DSKey.DPAD_DOWN,
            "DPAD_LEFT": DSKey.DPAD_LEFT,
            "DPAD_RIGHT": DSKey.DPAD_RIGHT,

            "TOUCH": DSKey.TOUCH,
        }

        return mapping.get(key_str)

@dataclass
class Input:
    buttons: int
    cpad: Vector2D
    cpad_unscaled: Vector2D
    cstick: Vector2D
    cstick_unscaled: Vector2D
    touch: Vector2D
    touch_unscaled: Vector2D

    DS_PAD_MAX = 156
    XBOX_PAD_MAX = 32767
    DS_TOUCH_SCREEN_WIDTH = 320
    DS_TOUCH_SCREEN_HEIGHT = 240
    MONITOR_SIZE: Rect | None = None

    @staticmethod
    def init_monitor_size() -> bool:
        monitors = screeninfo.get_monitors()
        if len(monitors) < 1:
            return False

        monitor = monitors[0]
        
        Input.MONITOR_SIZE = Rect(monitor.width, monitor.height)
        return True

    @staticmethod
    def from_packet(packet: Network.Packet, touch_sensitivity: float, linux_screen_scale: float) -> "Input":
        buttons, cpad_x, cpad_y, cstick_x, cstick_y, touch_x, touch_y = packet[0 : 7]
        
        state = Input(
            buttons,
            Input.scale_stick(cpad_x, cpad_y),
            Input.scale_stick(cstick_x, cstick_y),
            Vector2D(cpad_x, cpad_y),
            Vector2D(cstick_x, cstick_y),
            Input.scale_touch(touch_x, touch_y, touch_sensitivity * (linux_screen_scale if Utils.is_linux() else 1)),
            Vector2D(touch_x, touch_y)
        )

        return state

    @staticmethod
    def scale_stick(x: int, y: int) -> Vector2D:
        scaled_x = max(min(int(x / Input.DS_PAD_MAX * Input.XBOX_PAD_MAX), Input.XBOX_PAD_MAX), -Input.XBOX_PAD_MAX - 1)
        scaled_y = max(min(int(y / Input.DS_PAD_MAX * Input.XBOX_PAD_MAX), Input.XBOX_PAD_MAX), -Input.XBOX_PAD_MAX - 1)

        return Vector2D(scaled_x, scaled_y)

    @staticmethod
    def scale_touch(x: int, y: int, sensitivity: float) -> Vector2D:
        assert Input.MONITOR_SIZE is not None

        clamped_x = max(0, min(x, Input.DS_TOUCH_SCREEN_WIDTH))
        clamped_y = max(0, min(y, Input.DS_TOUCH_SCREEN_HEIGHT))

        # Scale to monitor
        scaled_x = clamped_x / Input.DS_TOUCH_SCREEN_WIDTH * Input.MONITOR_SIZE.width
        scaled_y = clamped_y / Input.DS_TOUCH_SCREEN_HEIGHT * Input.MONITOR_SIZE.height

        # Monitor center
        center_x = Input.MONITOR_SIZE.width / 2
        center_y = Input.MONITOR_SIZE.height / 2

        # Apply sensitivity to distance relative to center
        offset_x = (scaled_x - center_x) * sensitivity
        offset_y = (scaled_y - center_y) * sensitivity

        final_x = int(center_x + offset_x)
        final_y = int(center_y + offset_y)

        return Vector2D(final_x, final_y)

    def is_touch(self) -> bool:
        return bool(self.buttons & DSKey.TOUCH)
