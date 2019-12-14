from os import system
from decision import python3


if python3:
    from installjpm import *

def install():
    system("sudo mv jaclang /usr/local/bin/jaclang")

    system("sudo python3 install/makealias.py")
    if python3:
        installjpm_main()
    else:
        system("python3 install/installjpm.py")
    print()
    print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
