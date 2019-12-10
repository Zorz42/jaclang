from __future__ import print_function
from os import system


def install():
    system("sudo mv jaclang /usr/local/bin/jaclang")

    system("sudo python3 install/makealias.py")
    print()
    print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
