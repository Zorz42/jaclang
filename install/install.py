from os import system
import platform


def install():
    system("sudo mv jaclang /usr/local/bin/jaclang")
    system("sudo cp -r jaclang-data /usr/local/bin/")

    system("sudo python3 install/makealias.py")
    if platform.system() == 'Linux':
        system("sudo cp data/jpm.bash /etc/bash_completion.d/")
    print()
    print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
