from __future__ import print_function

from os import system, path, listdir, popen
from platform import system as sys
from subprocess import call
from sys import stdout
from threading import Thread

objdir = "obj/"
srcdir = "src/"
includedir = "include/"
stdlib = "gnu++17"
optimisation = "fast"
warnings = "-Wall -Wshadow -Wextra -Wno-deprecated"

compiled_count = 0

return_fail = False

osplatform = None

if sys() == "Linux":
    osplatform = "linux"
elif sys() == "Darwin":
    osplatform = "OSX"
else:
    print("Unsupported platform!")
    exit(1)

def getColumns():
    return int(popen('stty size', 'r').read().split()[1])

def clearBar():
    for i in range(getColumns()):
        print(" ", end='')
    print("\r", end='')

def print_progress_bar(compiled, total, length):
    print('[', end='')
    for i in range(int(length / total * compiled)):
        print("#", end='')
    for i in range(int(length / total * (total - compiled))):
        print(" ", end='')
    print("]\r", end='')
    stdout.flush()

class BuildThread(Thread):
    def __init__(self, filename):
        Thread.__init__(self)
        self.name = filename

    def run(self):
        global compiled_count
        if osplatform == "linux":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{includedir} -I{objdir} " + \
                              f"-o {objdir}{self.name}.o -c {srcdir}{self.name}.cpp"
        elif osplatform == "OSX":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{includedir} -o {objdir}{self.name}.o " + \
                              f"-c {srcdir}{self.name}.cpp -include-pch {objdir}jaclang.h.gch -D IGNORE_MAIN_INCLUDE"
        current_thread = call(compile_command, shell=True)
        if current_thread != 0:
            success = "FAILED"
            global return_fail
            return_fail = True
        else:
            success = "OK"
            compiled_count += 1
        columns = getColumns()
        for i in range(columns):
            print(" ", end='')
        print("\r", end='')
        print(f"[{success}] [GCC] [-FLAGS] {srcdir}{self.name}.cpp -> {objdir}{self.name}.o")
        print_progress_bar(compiled_count, self.threadID, columns - 2)

def build():
    print()

    if not path.isdir(objdir):
        system(f"mkdir {objdir}")
    files = [file.split('.')[0] for file in listdir(srcdir) if not path.isdir(f"{srcdir}{file}")]
    srcfilesdirs = [Dir for Dir in listdir(srcdir) if len(Dir.split('.')) == 1]

    for Dir in srcfilesdirs:
        if not path.isdir(objdir + Dir):
            system(f"mkdir {objdir}{Dir}")
        files += [Dir + "/" + str(file.split('.')[0]) for file in listdir(srcdir + Dir) if len(file.split('.')) == 2]

    threads = []
    build_header = False

    for file in listdir(includedir):
        if not path.isfile(f"{objdir}jaclang.h.gch") or path.getctime(f"{objdir}jaclang.h.gch") < \
                path.getctime(includedir + file):
            build_header = True

    for file in files:
        if build_header or not path.isfile(f"{objdir}{file}.o") or \
        path.getctime(f"{objdir}{file}.o") < path.getctime(f"{srcdir}{file}.cpp"):
            threads.append(BuildThread(file))

    if build_header:
        print("Building jaclang header...")
        if osplatform == "linux":
            system(f"g++ {warnings} -c -O{optimisation} {includedir}jaclang.h -o {objdir}jaclang.h.gch -std={stdlib}")
        elif osplatform == "OSX":
            system(f"g++ {warnings} -c -O{optimisation} {includedir}jaclang.h -o {objdir}jaclang.h.gch -std={stdlib}")

    if threads:
        print("Building jaclang...")
        for thread in threads:
            thread.threadID = len(threads)
            thread.start()
        for thread in threads:
            thread.join()
            if return_fail:
                exit(1)
        clearBar()

    objfiles = [objdir + file + ".o" for file in files]
    print("Linking object files... ")
    if osplatform == "linux":
        linker_return_code = call(f"g++ -m64 -std={stdlib} -o jaclang " + " ".join(objfiles), shell=True)
    elif osplatform == "OSX":
        linker_return_code = call(f"g++ -m64 -std={stdlib} -o jaclang " + " ".join(objfiles), shell=True)

    if linker_return_code != 0:
        print("Linking failed!")
        exit(1)


if __name__ == "__main__":
    build()
