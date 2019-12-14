from __future__ import print_function
from os import system, popen, environ, path
import platform

from decision import *

bin_paths = environ["PATH"].split(":")

if python3:
    from checkforpippackages import *
    from installjpm import *

if python3:
    print("Installing using python3")
else:
    print("Installing using python2")


def check_for_package(name, binary, install_command):
    print(name.upper() + ' ... ', end='')
    exists = False
    for bin_path in bin_paths:
        if path.isfile(bin_path + "/" + binary):
            exists = True
            break
    if not exists:
        print("FAILED")
        if decision("Do you want me to install " + name + "?"):
            system(install_command)
        else:
            exit(1)
    else:
        print("OK")


def dependencies():
    system("sudo echo")
    if platform.system() == 'Linux':
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

        print("Checking for dependencies:")
        if current_package_manager.split(" ")[0] == 'zypper':
            check_for_package("g++", "g++", "sudo " + current_package_manager + " gcc-c++")
        else:
            check_for_package("g++", "g++", "sudo " + current_package_manager + " g++")
        packages = [
            ("nasm", "nasm"),
            ("binutils", "ld"),
            ("unzip", "unzip"),
            ("python3", "python3"),
            ("python3-pip", "pip3"),
        ]
        for package in packages:
            check_for_package(package[0], package[1], "sudo " + current_package_manager + package[0])

    elif platform.system() == 'Darwin':

        print("Checking for dependencies:")
        check_for_package("brew", "brew",
                          '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/'
                          'Homebrew/install/master/install)"')
        packages = [
            ("nasm", "nasm"),
            ("gcc", "g++"),
            ("binutils", "ld"),
            ("unzip", "unzip"),
            ("python3", "python3"),
        ]
        for package in packages:
            check_for_package(package[0], package[1], "brew install " + package[0])
    else:
        print("Unsupported os!")
        exit(1)

    if python3:
        checkforpippackages_main()
        installjpm_main()
    else:
        system("python3 install/checkforpippackages.py")
        system("python3 install/installjpm.py")
