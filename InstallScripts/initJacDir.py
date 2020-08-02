from os import makedirs
from platform import system as sys

install_dir = "/usr/local/Jac/"


def initJacDir():
    dirs = ["Binaries", "Data", "ToInstall", "Libraries"]
    for dir_ in dirs:
        makedirs(f"{install_dir}{dir_}", exist_ok=True)

    if sys() == "Darwin":
        with open("/etc/paths", "r+") as paths_file:
            if f"{install_dir}Binaries" not in paths_file.read().split("\n"):
                paths_file.write(f"{install_dir}Binaries\n")
    elif sys() == "Linux":
        pass
    else:
        print("Unsupported os!")
        exit(1)
