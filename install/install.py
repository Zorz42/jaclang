from os import system
from platform import system as sys


def install():
    system("sudo mv jaclang /usr/local/bin/jaclang")
    system("sudo cp -r data/sjaclang-data /usr/local/share/")

    if sys() == 'Linux':
        system("sudo cp data/jpm.bash /etc/bash_completion.d/")
    print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")


if __name__ == "__main__":
    install()
