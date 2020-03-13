from os import system
from pkgutil import iter_modules

yesOptions = ["Y", "YES"]
noOptions = ["N", "NO"]

packages = [name for loader, name, ispkg in iter_modules()]


def check_for_pip_package(name):
    print("PIP3-" + name.upper() + " ... ", end='', flush=True)
    if name in packages:
        print("OK")
    else:
        print("FAILED")
        while True:
            decision = input("Do you want me to install pip3-" + name + "? [y,n]")
            if decision.upper() in yesOptions:
                system("sudo pip3 install " + name)
                break
            elif decision.upper() in noOptions:
                exit(1)


def checkforpippackages_main():
    needed_packages = [
        "setuptools",
        "wget",
        "httplib2",
        "zipfile",
    ]
    for package in needed_packages:
        check_for_pip_package(package)


if __name__ == "__main__":
    checkforpippackages_main()
