from os import listdir, path, remove
from platform import system as sys
from shutil import move, copyfile, which, copy

from initJacDir import initJacDir, install_dir


def install():
    print("Installing Jaclang.")
    initJacDir()

    move("jaclang", f"{install_dir}Binaries/jaclang")
    for file in listdir("Data"):
        copyfile(f"Data/{file}", f"{install_dir}Data/{file}")

    if sys() == "Darwin":
        if path.isfile("/etc/paths.d/jaclang-paths"):
            remove("/etc/paths.d/jaclang-paths")
        copy(f"{install_dir}Data/jaclang-paths.txt", "/etc/paths.d/jaclang-paths")
        remove(f"{install_dir}Data/jaclang-paths.txt")
    elif sys() == "Linux":
        with open(f"{path.expanduser('~')}/.bashrc") as bashrc_file:
            lines = bashrc_file.read().split("\n")
            path_line = "export PATH=${PATH}:/usr/local/Jac/Binaries"
            if path_line not in lines:
                bashrc_file.write(f"{path_line}\n")
    else:
        print("Unsupported os!")
        exit(1)

    print("Jaclang is installed.", end='')

    if which("jaclang") is None:
        print(" If you want to use it you will have to restart your terminal session or run:")
        print("     eval $(make _set)")
    else:
        print()


if __name__ == "__main__":
    install()
