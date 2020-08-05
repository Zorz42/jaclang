from os import makedirs

install_dir = "/usr/local/Jac/"


def initJacDir():
    dirs = ["Binaries", "Data", "Libraries"]
    for dir_ in dirs:
        makedirs(f"{install_dir}{dir_}", exist_ok=True)