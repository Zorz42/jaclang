from sys import argv
from os import popen

curr_word = int(argv[1])
args = argv[3:]


def main():
    if curr_word == 1:
        return ["install", "remove", "list", "cleanup", "upgrade", "repair", "listall", "version"]
    elif curr_word == 2:
        if args[0] == "install":
            packages = popen("jpm listall").read().split("\n")[4].split(" ")
            packages = [package for package in packages if package != ""]
            for i in range(len(packages)):
                while packages[i][0] != 'm':
                    packages[i] = packages[i][1:]
                packages[i] = packages[i][1:]
            packages = [package for package in packages if package != ""]
            return packages
        elif args[0] == "remove":
            return [package[4:] for package in popen("jpm list").read().split("\n")[2:]]
        elif args[0] == "upgrade":
            return []
        else:
            return []
    else:
        return []


if __name__ == "__main__":
    print(" ".join(main()))
