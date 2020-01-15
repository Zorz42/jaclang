from os import system


def install():
    system("sudo mv jaclang /usr/local/bin/jaclang")
    system("sudo cp -r jaclang-data /usr/local/bin/")

    system("sudo python3 install/makealias.py")
    print()
    print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
