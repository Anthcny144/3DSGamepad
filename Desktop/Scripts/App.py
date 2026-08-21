import keyboard as kb
import time
import socket
import sys
from colorama import Fore as F
from threading import Thread
from .Config import Config
from .Input import Input
from .InputManager import InputManager, ControllerType
from .Network import Network
from .Utils import Utils

class App:
    thread = None
    config: Config | None = None

    @staticmethod
    def init() -> bool:
        # Check OS
        if Utils.is_unsupported_os():
            print(f"Platform / OS \"{sys.platform}\" is not supported.")
            return False

        # Read monitor size
        if not Input.init_monitor_size():
            return False

        # Read config
        App.config = Config.load(Config.DEFAULT_PATH if len(sys.argv) == 1 else sys.argv[1])
        if App.config is None:
            return False

        # Ignore screen scale on Windows
        # There is no official way to do this on Linux,
        # hence why the LINUX_SCREEN_SCALE setting
        if Utils.is_windows():
            import ctypes
            ctypes.windll.user32.SetProcessDPIAware()

        # Init sock
        Network.init(App.config.TIMEOUT)

        # Start stopping thread
        App.thread = Thread(target=App.stopper)
        App.thread.start()

        # Gamepad
        InputManager.init(App.config.CONTROLLER_TYPE, App.config.KEYBOARD_CONFIG)

        return True

    @staticmethod
    def stopper() -> None:
        while True:
            time.sleep(1 / 60)
            assert App.config is not None

            if not kb.is_pressed(App.config.EXIT):
                continue

            if Network.sock is not None:
                Network.sock.close()
                Network.sock = None

            break

    @staticmethod
    def run() -> None:
        assert App.config is not None
        App.print_config()
        print(f"{F.CYAN}Waiting for the console to connect on port {F.YELLOW}{Network.PORT}{F.LIGHTBLUE_EX}...{F.RESET}")
        
        while True:
            try:
                packet = Network.wait_for_packet()
                if not Network.confirm_receive(packet): # If first receive
                    print(f"{F.LIGHTGREEN_EX}Inputs are being received! {F.CYAN}Press {F.YELLOW}{App.config.EXIT.title()}{F.CYAN} to stop listening for inputs.{F.RESET}")

            except socket.timeout:
                if App.reset():
                    print(f"\n{F.LIGHTRED_EX}The console disconnected. {F.CYAN}Waiting for reconnection...{F.RESET}")

                continue

            # Force exit by the stopping thread
            except OSError:
                App.reset()
                break

            state = Input.from_packet(packet, App.config.KEYBOARD_CONFIG.TOUCH_SENSITIVITY, App.config.KEYBOARD_CONFIG.LINUX_SCREEN_SCALE)                
            InputManager.inject(state)

    @staticmethod
    def print_config() -> None:
        assert InputManager.controller_type is not None and App.config is not None
        print(f"{F.LIGHTBLUE_EX}Controller type: {F.YELLOW}{InputManager.controller_type.title()}{F.RESET}")

        if InputManager.controller_type == ControllerType.KEYBOARD:
            print(f"{F.LIGHTBLUE_EX}Click mode: {F.YELLOW}{App.config.KEYBOARD_CONFIG.CLICK_MODE.title()}{F.RESET}")

    @staticmethod
    def reset() -> bool:
        InputManager.reset()
        return Network.reset()