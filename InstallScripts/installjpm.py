from os import path, remove, listdir, system
from shutil import copytree, rmtree, copyfile
from tarfile import open as tar_open
from urllib.request import urlopen

from initJacDir import initJacDir, install_dir

jpm_version = "1.7.12"


def install_jpm_main():
    print("Installing Jpm.")
    with urlopen(f"https://github.com/Zorz42/jpm/archive/v{jpm_version}.tar.gz") as f:
        html = f.read()
    with open("jpm.tar.gz", "wb") as file:
        file.seek(0)
        file.write(html)
        file.truncate()

    with tar_open(f"jpm.tar.gz", "r:gz") as tar_file:
        tar_file.extractall()

    initJacDir()

    if path.isdir(f"{install_dir}Jpm"):
        rmtree(f"{install_dir}Jpm")
    copytree(f"jpm-{jpm_version}/Sources", f"{install_dir}Jpm")

    for file in listdir(f"jpm-{jpm_version}/Data"):
        copyfile(f"jpm-{jpm_version}/Data/{file}", f"{install_dir}Data/{file}")

    if path.isfile(f"{install_dir}Binaries/jpm"):
        remove(f"{install_dir}Binaries/jpm")

    copyfile(f"jpm-{jpm_version}/jpm", f"{install_dir}Binaries/jpm")
    system(f"sudo chmod +x {install_dir}Binaries/jpm")

    remove("jpm.tar.gz")
    rmtree(f"jpm-{jpm_version}")


if __name__ == "__main__":
    install_jpm_main()
