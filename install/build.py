from __future__ import print_function
from os import system, path, listdir, popen
from subprocess import Popen

import threading
import sys

from settings import *

compiled_count = 0

def buildfile(filename, objlen):
    global compiled_count
    currentThread = Popen(
        ("g++ -w -pipe -m64 -std=gnu++11 -I" + includedir + " -o " + objdir + "/" + filename + ".o -c " +
        srcdir + "/" + filename + ".cpp").split(" "))
    while currentThread.poll() is None:
        pass
    columns = int(popen('stty size', 'r').read().split()[1]) - 2
    for i in range(columns + 2):
        print(" ", end='')
    print("\r", end='')
    print("[CC] [-FLAGS] " + srcdir + "/" + filename + ".cpp -> " + objdir + "/" + filename + ".o")
    compiled_count += 1
    print_progress_bar(compiled_count, objlen, columns)

class buildThread(threading.Thread):
    def __init__(self, count, filename):
       threading.Thread.__init__(self)
       self.threadID = count
       self.name = filename
       self.objlen = 0
    def run(self):
       buildfile(self.name, self.objlen)

def print_progress_bar(compiled, total, length):
    print('[', end='')
    for i in range(int(length / total * compiled)):
        print("#", end='')
    for i in range(int(length / total * (total - compiled))):
        print(" ", end='')
    print("]\r", end='')
    sys.stdout.flush()


def build(fail=False):
    print("Preparing build...")
    if not fail:
        system("sudo echo")
    if not path.isdir(objdir):
        system("mkdir " + objdir)
    objectfiles = [file.split('.')[0] for file in listdir(srcdir)]
    count = 0
    threads = []
    for file in objectfiles:
        count += 1
        if not path.isfile(objdir + "/" + file + ".o") or path.getctime(objdir + "/" + file + ".o") < path.getctime(srcdir + "/" + file + ".cpp"):
            threads.append(buildThread(count, file))
    print("Building jaclang. Please wait...")
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
    for file in objectfiles:
        objnames += objdir + "/" + file + ".o "
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