from sys import argv
from os import listdir, path

curr_word = int(argv[1])
args = argv[3:]

def listcurrdir():
    return [file for file in listdir(".") if (file.endswith(".jl") or
        file.endswith(".asm") or file.endswith(".o")) and not path.isdir(file)]
        
def listcurrdir2():
    return [file for file in listdir(".") if (file.endswith(".asm") or
        file.endswith(".o") or "." not in file) and not path.isdir(file)]

def main():
    if curr_word == 1:
        return ["version", "versionid", "versionstr"] + listcurrdir()
    elif curr_word == 2:
        return listcurrdir2()
    else:
        return []

if __name__ == "__main__":
    print(" ".join(main()))
