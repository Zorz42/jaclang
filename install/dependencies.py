from __future__ import print_function

from decision import decision, python3
from os import popen, environ, path, system
from platform import system as sys

packages_to_install = []

bin_paths = environ["PATH"].split(":")

if python3:
    from checkforpippackages import checkforpippackages_main


def check_for_package(name, binary, install_command):
    print(name.upper() + ' ... ', end='')
    for bin_path in bin_paths:
        if path.isfile(bin_path + "/" + binary):
            print("OK")
            break
    else:
        print("FAILED")
        if decision("Do you want me to install " + name + "?"):
            system(install_command)
        else:
            exit(1)


def add_package(package_name, binary):
    print(package_name.upper() + ' ... ', end='')
    for bin_path in bin_paths:
        if path.isfile(bin_path + "/" + binary):
            print("OK")
            break
    else:
        print("FAILED")
        packages_to_install.append(package_name)


def dependencies():
    system("sudo echo")
    print("Checking for dependencies:")
    if sys() == 'Linux':
        current_package_manager = ''
        package_managers = ['zypper', 'apt', 'yum', 'emerge', 'pacman']
        for package_manager in package_managers:
            if popen("which " + package_manager).read() != "":
                if package_manager == 'apt':
                    current_package_manager = 'apt install '
                elif package_manager == 'yum':
                    current_package_manager = 'yum install '
                elif package_manager == 'emerge':
                    current_package_manager = 'emerge '
                elif package_manager == 'pacman':
                    current_package_manager = 'pacman -S '
                elif package_manager == 'zypper':
                    current_package_manager = 'zypper in '
                break
        if current_package_manager == '':
            print('Could not find package manager!')
            exit(1)

        if current_package_manager.split(" ")[0] == 'zypper':
            check_for_package("g++", "g++", "sudo " + current_package_manager + " gcc-c++")
        else:
            check_for_package("g++", "g++", "sudo " + current_package_manager + " g++")
        packages = (
            ("python3", "python3"),
            ("python3-pip", "pip3"),
        )
        for package in packages:
            add_package(package[0], package[1])

        if packages_to_install:
            if decision("Do you want me to install " + " ".join(packages_to_install) + "?"):
                print("Installing...")
                system("sudo " + current_package_manager + " ".join(packages_to_install))
            else:
                print("Aborting")
                exit(1)

    elif sys() == 'Darwin':
        check_for_package("brew", "brew",
                          '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/'
                          'Homebrew/install/master/install)"')
        check_for_package("gcc", "gcc", 'xcode-select --install')
        packages = (
            ("python3", "python3"),
        )
        for package in packages:
            add_package(package[0], package[1])

        if packages_to_install:
            if decision("Do you want me to install " + "".join(packages_to_install) + "?"):
                print("Installing...")
                system("brew install " + " ".join(packages_to_install))
            else:
                print("Aborting")
                exit(1)
    else:
        print("Unsupported os!")
        exit(1)

    if python3:
        checkforpippackages_main()
    else:
        system("python3 install/checkforpippackages.py")


if __name__ == "__main__":
    dependencies()
