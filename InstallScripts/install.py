from os import listdir, path, remove, system
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

    if sys() == "Darwin":
        if path.isfile("/etc/paths.d/jaclang-paths"):
            remove("/etc/paths.d/jaclang-paths")
        copy(f"{install_dir}Data/jaclang-paths.txt", "/etc/paths.d/jaclang-paths")
    elif sys() == "Linux":
        if path.isfile("/etc/profile.d/jaclang-paths.sh"):
            remove("/etc/profile.d/jaclang-paths.sh")
        copy(f"{install_dir}Data/jaclang-paths.sh", "/etc/profile.d/jaclang-paths.sh")
    else:
        print("Unsupported os!")
        exit(1)

    remove(f"{install_dir}Data/jaclang-paths.txt")
    remove(f"{install_dir}Data/jaclang-paths.sh")
    print("Jaclang is installed.", end='')

    if which("jaclang") is None:
        print(" If you want to use it you will have to restart your terminal session or run:")
        print("     eval $(make _set)")
    else:
        print()


if __name__ == "__main__":
    install()
