from os import path
from platform import system as sys
from shutil import move, copytree, copyfile, rmtree


def install():
    move("jaclang", "/usr/local/bin/jaclang")
    if path.isdir("/usr/local/share/jaclang-data"):
        rmtree("/usr/local/share/jaclang-data")
    copytree("data/jaclang-data", "/usr/local/share/jaclang-data")

    if sys() == 'Linux':
        copyfile("data/jpm.bash", "/etc/bash_completion.d/")
    print("Jaclang installed successfully! Type jaclang in terminal for help.")


if __name__ == "__main__":
    install()
