from os import system, path, remove
from urllib.request import urlopen
from tarfile import open as tar_open
from shutil import copytree, rmtree, copyfile

install_folder = "/usr/local/share/"
jacmake_version = "1.0.6"


def install_jacmake_main():
    print("Installing jacmake.")
    with urlopen(f"https://github.com/Zorz42/jacmake/archive/v{jacmake_version}.tar.gz") as f:
        html = f.read()
    with open("newerjacmake.tar.gz", "wb") as file:
        file.seek(0)
        file.write(html)
        file.truncate()

    with tar_open(f"newerjacmake.tar.gz", "r:gz") as tar_file:
        tar_file.extractall()

    if path.isdir(f"{install_folder}jacmake-sources"):
        rmtree(f"{install_folder}jacmake-sources")
    copytree(f"jacmake-{jacmake_version}/jacmake-sources", f"{install_folder}jacmake-sources")

    if path.isfile("/usr/local/bin/jacmake"):
        remove("/usr/local/bin/jacmake")
    copyfile(f"jacmake-{jacmake_version}/jacmake", "/usr/local/bin/jacmake")
    system("sudo chmod +x /usr/local/bin/jacmake")

    remove("newerjacmake.tar.gz")
    rmtree(f"jacmake-{jacmake_version}")


if __name__ == "__main__":
    install_jacmake_main()

