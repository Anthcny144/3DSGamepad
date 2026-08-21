import sys
import time
from dataclasses import dataclass

class Utils:
    @staticmethod
    def is_windows() -> bool:
        return sys.platform == "win32"

    @staticmethod
    def is_linux() -> bool:
        return sys.platform == "linux"

    @staticmethod
    def is_unsupported_os() -> bool:
        return sys.platform == "darwin" or sys.platform not in ("win32", "linux")

class Clock:
    def __init__(self) -> None:
        self.reset()

    def is_elapsed(self, ms: float) -> bool:
        now = time.perf_counter()
        res = now - self.time >= ms / 1000

        if res:
            self.reset()

        return res

    def reset(self) -> None:
        self.time = time.perf_counter()

@dataclass
class Vector2D:
    x: int
    y: int

@dataclass
class Rect:
    width: int
    height: int