from os import system, path, remove, listdir
from urllib.request import urlopen
from tarfile import open as tar_open
from shutil import copytree, rmtree, copyfile

from initJacDir import initJacDir, install_dir

jacmake_version = "1.1.2"


def install_jacmake_main():
    print("Installing Jacmake.")
    with urlopen(f"https://github.com/Zorz42/jacmake/archive/v{jacmake_version}.tar.gz") as f:
        html = f.read()
    with open("jacmake.tar.gz", "wb") as file:
        file.seek(0)
        file.write(html)
        file.truncate()

    with tar_open(f"jacmake.tar.gz", "r:gz") as tar_file:
        tar_file.extractall()

    initJacDir()

    if path.isdir(f"{install_dir}Jacmake"):
        rmtree(f"{install_dir}Jacmake")
    copytree(f"jacmake-{jacmake_version}/Sources", f"{install_dir}Jacmake")

    for file in listdir(f"jacmake-{jacmake_version}/Data"):
        copyfile(f"jacmake-{jacmake_version}/Data/{file}", f"{install_dir}Data/{file}")

    if path.isfile(f"{install_dir}Binaries/jacmake"):
        remove(f"{install_dir}Binaries/jacmake")
    
    copyfile(f"jacmake-{jacmake_version}/jacmake", f"{install_dir}Binaries/jacmake")
    system(f"sudo chmod +x {install_dir}Binaries/jacmake")

    remove("jacmake.tar.gz")
    rmtree(f"jacmake-{jacmake_version}")


if __name__ == "__main__":
    install_jacmake_main()

