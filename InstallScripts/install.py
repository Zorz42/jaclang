from os import listdir, path, remove, system, symlink
from platform import system as sys
from shutil import move, copyfile, which, copy

from initJacDir import initJacDir, install_dir


def install():
    if not path.isfile("jaclang"):
        print("You must first successfully build jaclang and then install it!")
        exit(1)

    print("Installing Jaclang.")
    initJacDir()

    move("jaclang", f"{install_dir}Binaries/jaclang")
    for file in listdir("Data"):
        copyfile(f"Data/{file}", f"{install_dir}Data/{file}")
    
    if path.isfile(f"/usr/local/bin/jaclang"):
        remove(f"/usr/local/bin/jaclang")
    symlink(f"{install_dir}Binaries/jaclang", f"/usr/local/bin/jaclang")

    print("Jaclang is installed.")


if __name__ == "__main__":
    install()
