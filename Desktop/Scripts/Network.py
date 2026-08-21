import socket
import struct
import time
from colorama import Fore as F
from .Utils import Clock

class Network:
    type SockResponse = tuple[bytes, tuple[str, int]]
    type Packet = tuple[int, int, int, int, int, int, int, int, int]
    sizeof_packet = 24

    PORT = 14418
    sock: socket.socket | None = None
    last_now: float | None = None
    force_next_print = False
    clock: Clock = Clock()

    @staticmethod
    def init(timeout: float) -> None:
        Network.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        Network.sock.bind(("0.0.0.0", Network.PORT))
        Network.sock.settimeout(timeout)

    @staticmethod
    def wait_for_packet() -> Packet:
        assert Network.sock is not None
        response: Network.SockResponse = Network.sock.recvfrom(Network.sizeof_packet)
        data, _ = response
        return struct.unpack("<IhhhhhhII", data)

    @staticmethod
    def confirm_receive(packet: Packet) -> bool:
        last = Network.last_now
        now = time.perf_counter()
        Network.last_now = now

        if last is None:
            return False

        elapsed = (now - last) * 1000

        if Network.clock.is_elapsed(100) or Network.force_next_print:
            Network.force_next_print = False
            frequency = packet[7]

            loop_delay = 1000 / frequency
            network_delay = elapsed - loop_delay

            # Sometime packets arrive late. If that happens, the next one will
            # arrive very early. This check prevents showing some of those
            # unwanted spikes, though this is not perfect
            if network_delay <= 0:
                Network.force_next_print = True
                return True

            print(f"\r{F.LIGHTBLUE_EX}Input latency: {F.YELLOW}{round(elapsed, 2)}ms    \t{F.LIGHTBLUE_EX}Network latency: {F.YELLOW}{round(network_delay, 2)}ms    \t", end=f"{F.RESET}", flush=True)

        return True

    @staticmethod
    def reset() -> bool:
        res = Network.last_now is not None
        Network.last_now = None
        Network.force_next_print = False
        return res