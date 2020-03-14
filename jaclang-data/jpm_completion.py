from os import popen
from sys import argv

curr_word = int(argv[1])
args = argv[3:]


def main():
    if curr_word == 1:
        return ["install", "remove", "list", "cleanup", "upgrade", "repair", "listall", "version"]
    elif curr_word == 2:
        if args[0] == "install":
            with open("/usr/local/share/jaclang-data/jpm-database.txt") as cacheFile:
                packages = cacheFile.read().split("\n")
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
