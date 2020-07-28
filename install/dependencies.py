from __future__ import print_function
from os import environ, path, system
from platform import system as sys

from decision import decision

packages_to_install = []

bin_paths = environ["PATH"].split(":")


def checkForBinary(binary):
    for bin_path in bin_paths:
        if path.isfile(bin_path + "/" + binary):
            return True
    return False


def addPackage(package_name, binary):
    print(package_name.upper() + " ... ", end="")
    for bin_path in bin_paths:
        if path.isfile(bin_path + "/" + binary):
            print("OK")
            break
    else:
        print("FAILED")
        packages_to_install.append(package_name)


def dependencies():
    current_package_manager, packages = "", ()
    if sys() == "Linux":
        package_managers = (("zypper", "in"), ("apt", "install"), ("yum", "install"), ("emerge", ""), ("pacman", "-S"))
        for package_manager, install_command in package_managers:
            if checkForBinary(package_manager):
                current_package_manager = "sudo " + package_manager + " " + install_command + " "
                break

        if not current_package_manager:
            print("Could not find package manager!")
            exit(1)

        packages = (
            ("g++", "gcc-c++" if current_package_manager.startswith("zypper") else "g++"),
            ("python3", "python3"),
            ("python3-pip", "pip3"),
        )

    elif sys() == "Darwin":
        if not checkForBinary("brew") and \
                decision("Do you want to install brew?"):
            system(
                "/usr/bin/ruby -e \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)\"")
        packages = (
            ("python3", "python3"),
        )
        current_package_manager = "brew install "
    else:
        print("Unsupported os!")
        exit(1)

    for package_name, binary in packages:
        addPackage(package_name, binary)

    if packages_to_install:
        if decision("Do you want me to install " + " ".join(packages_to_install) + "?"):
            system(current_package_manager + " ".join(packages_to_install))
        else:
            exit(1)


if __name__ == "__main__":
    dependencies()
