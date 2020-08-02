from os import listdir
from platform import system as sys
from shutil import move, copyfile

from initJacDir import initJacDir, install_dir


def install():
    initJacDir()

    move("jaclang", f"{install_dir}Binaries/jaclang")
    for file in listdir("Data"):
        copyfile(f"Data/{file}", f"{install_dir}Data/{file}")

    if sys() == 'Linux':
        copyfile("data/jpm.bash", "/etc/bash_completion.d/")
    print("Jaclang installed successfully! Type jaclang in terminal for help.")


if __name__ == "__main__":
    install()
