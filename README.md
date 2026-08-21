# 3DSGamepad
This application allows using your 3DS as a controller for your computer.

# Notes
- Your 3DS and computer must be connected to the same network.
- This application is supported by <b>Windows and Linux only</b>.

## Installation
- [CFW](https://3ds.hacks.guide/) must be installed on your 3DS.
- [Python](https://www.python.org/) 3.12 or higher, as well as pip.
- Download and extract the zip folder from the [latest release](https://github.com/Anthcny144/3DSGamepad/releases/latest).
- Move the `3DSGamepad.3dsx` file to your SD card, usually in the `3ds` folder.
- Install the required Python packages: `pip install -r /path/to/requirements.txt`.
> [!NOTE]
> On Linux, you will need [gcc](https://gcc.gnu.org/) to build [uinput](https://pypi.org/project/python-uinput/).
> <br>
> On Windows, you will be prompted to install the [Nefarius Virtual Gamepad Emulation Bus Driver](https://vigembusdriver.com/), which is the driver used for creating Windows virtual gamepads.

## Configuring the desktop application
- Open the `config.yaml` file to customize the configuration. Read comments crefully as there are multiple configurations possible.
- You can use your 3DS as a virtual gamepad (detected by your system) or as a way to inject keyboard keys.

## Configuring the 3DS application
- Set the IP of your computer (destination IP). To get it, open a command prompt and type `ipconfig` on Windows or `ip a` on Linux, and look for the IPv4 address.
- You can increase or decrease how many packets are sent per second. Higher values mean lower input latency.

## Starting the application
- Run `Main.py` with Python.
    - The file takes 1 optional argument: the config file path. If no argument is given, it will automatically read the `config.yaml` file in the same directory.
    - Hold the exit key (`escape` by default) to stop listening for inputs.
> [!NOTE]
> On Linux, the application requires root permissions and X11 to run. If you are running Wayland, you will need to switch to an X11 session.
- Press "Connect" on the 3DS application to establish the connection.
    - Press "Disconnect" to stop sending inputs.
    
### About networking
The application sends and reads UDP packets on port 14418.