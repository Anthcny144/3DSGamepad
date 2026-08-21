import sys
from colorama import init as colorama_init
from Scripts.App import App

def main():
    colorama_init()
    if not App.init():
        sys.exit(1)

    App.run()
    sys.exit(0)

if __name__ == "__main__":
    main()
