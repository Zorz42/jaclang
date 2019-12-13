from __future__ import print_function
from os import system, path, listdir, popen
from subprocess import Popen

import threading
import sys

from settings import *

compiled_count = 0

platform = None

if sys.platform == "linux" or sys.platform == "linux2":
    platform = "linux"
elif sys.platform == "darwin":
    platform = "OSX"
else:
    print("Unsupported platform!")
    exit(1)


class BuildThread(threading.Thread):
    def __init__(self, count, filename):
        threading.Thread.__init__(self)
        self.threadID = count
        self.name = filename
        self.objlen = 0

    def run(self):
        global compiled_count
        compile_command = None
        if platform == "linux":
            compile_command = "g++ -w -pipe -m64 -std=gnu++11 -I" + includedir + " -I" + objdir + " -o " + objdir + \
                              self.name + ".o -c " + srcdir + self.name + ".cpp"
        elif platform == "OSX":
            compile_command = "g++ -w -pipe -m64 -std=gnu++11 -I" + includedir + " -o " + objdir + self.name + \
                              ".o -c " + srcdir + self.name + ".cpp -include-pch build/jaclang.h.gch"
        current_thread = Popen(compile_command.split(" "))
        while current_thread.poll() is None:
            pass
        columns = int(popen('stty size', 'r').read().split()[1]) - 2
        for i in range(columns + 2):
            print(" ", end='')
        print("\r", end='')
        print("[CC] [-FLAGS] " + srcdir + self.name + ".cpp -> " + objdir + self.name + ".o")
        compiled_count += 1
        print_progress_bar(compiled_count, self.objlen, columns)


def print_progress_bar(compiled, total, length):
    print('[', end='')
    for i in range(int(length / total * compiled)):
        print("#", end='')
    for i in range(int(length / total * (total - compiled))):
        print(" ", end='')
    print("]\r", end='')
    sys.stdout.flush()


def build(fail=False):
    print()
    print("Preparing build...")
    if not fail:
        system("sudo echo")
    if not path.isdir(objdir):
        system("mkdir " + objdir)
    files = [file.split('.')[0] for file in listdir(srcdir) if len(file.split('.')) == 2]
    srcfilesdirs = [Dir for Dir in listdir(srcdir) if len(Dir.split('.')) == 1]
    for Dir in srcfilesdirs:
        if not path.isdir("build/" + Dir):
            system("mkdir " + objdir + Dir)
        files += [Dir + "/" + str(file.split('.')[0]) for file in listdir(srcdir + Dir) if len(file.split('.')) == 2]
    count = 0
    threads = []
    for file in files:
        count += 1
        if not path.isfile(objdir + file + ".o") or path.getctime(objdir + file + ".o") < path.getctime(
                srcdir + file + ".cpp"):
            threads.append(BuildThread(count, file))
    for file in listdir(includedir):
        if not path.isfile(objdir + "jaclang.h.gch") or path.getctime(objdir + "jaclang.h.gch") < \
                path.getctime(includedir + file):
            print("Building jaclang header...")
            if platform == "linux":
                system("g++ -c " + includedir + "jaclang.h -o " + objdir + "jaclang.h.gch -w")
            elif platform == "OSX":
                system("g++ " + includedir + "jaclang.h -o " + objdir + "jaclang.h.gch -std=gnu++11 -stdlib=libc++ -w")
            threads = []
            for file2 in files:
                threads.append(BuildThread(count, file2))
            break
    print("Building jaclang...")
    for thread in threads:
        thread.objlen = len(threads)
        thread.start()
    for thread in threads:
        thread.join()
    columns = int(popen('stty size', 'r').read().split()[1]) - 2
    for i in range(columns + 2):
        print(" ", end='')
    print('\r', end='')

    objnames = ""
    for file in files:
        objnames += objdir + file + ".o "
    print("Linking object files ... ", end='')
    sys.stdout.flush()
    linker_return_value = Popen("g++ -W -o jaclang -m64 -std=gnu++11 " + objnames, shell=True)
    linker_return_value.communicate()
    linker_return_code = linker_return_value.returncode

    if linker_return_code != 0:
        if not fail:
            print("FAIL")
            print("Linker failed.")
            print("Recompiling the whole program from scratch.")
            system("make clean")
            build(True)
        else:
            print("Program failed twice. Aborting...")
            exit(1)
    else:
        print("DONE")
