from __future__ import print_function

from os import system, path, listdir, popen
from platform import system as sys
from subprocess import call
from sys import stdout
from threading import Thread

objdir = "obj/"
srcdir = "src/"
includedir = "include/"

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


class BuildThread(Thread):
    def __init__(self, count, filename):
        Thread.__init__(self)
        self.threadID = count
        self.name = filename
        self.objlen = 0

    def run(self):
        global compiled_count
        compile_command = None
        if osplatform == "linux":
            compile_command = "g++ -w -Ofast -pipe -m64 -std=gnu++11 -I" + includedir + " -I" + objdir + " -o " + objdir + \
                              self.name + ".o -c " + srcdir + self.name + ".cpp"
        elif osplatform == "OSX":
            compile_command = "g++ -w -pipe -m64 -std=gnu++11 -I" + includedir + " -o " + objdir + self.name + \
                              ".o -c " + srcdir + self.name + ".cpp -include-pch " + objdir + "jaclang.h.gch -Ofast"
        current_thread = call(compile_command, shell=True)
        columns = int(popen('stty size', 'r').read().split()[1]) - 2
        for i in range(columns + 2):
            print(" ", end='')
        print("\r", end='')
        print("[CC] [-FLAGS] " + srcdir + self.name + ".cpp -> " + objdir + self.name + ".o")
        if current_thread != 0:
            print("Compiling for " + srcdir + self.name + ".cpp failed. Aborting.")
            global return_fail
            return_fail = True
        else:
            compiled_count += 1
            print_progress_bar(compiled_count, self.objlen, columns)


def print_progress_bar(compiled, total, length):
    print('[', end='')
    for i in range(int(length / total * compiled)):
        print("#", end='')
    for i in range(int(length / total * (total - compiled))):
        print(" ", end='')
    print("]\r", end='')
    stdout.flush()


def build():
    print()
    print("Preparing build...")

    if not path.isdir(objdir):
        system("mkdir " + objdir)
    files = [file.split('.')[0] for file in listdir(srcdir) if len(file.split('.')) == 2]
    srcfilesdirs = [Dir for Dir in listdir(srcdir) if len(Dir.split('.')) == 1]

    for Dir in srcfilesdirs:
        if not path.isdir(objdir + Dir):
            system("mkdir " + objdir + Dir)
        files += [Dir + "/" + str(file.split('.')[0]) for file in listdir(srcdir + Dir) if len(file.split('.')) == 2]

    count = 0
    threads = []
    build_header = False

    for file in listdir(includedir):
        if not path.isfile(objdir + "jaclang.h.gch") or path.getctime(objdir + "jaclang.h.gch") < \
                path.getctime(includedir + file):
            build_header = True

    for file in files:
        count += 1
        if build_header or not path.isfile(objdir + file + ".o") or path.getctime(objdir + file + ".o") < path.getctime(
                srcdir + file + ".cpp"):
            threads.append(BuildThread(count, file))

    if build_header:
        print("Building jaclang header...")
        if osplatform == "linux":
            system("g++ -c " + includedir + "jaclang.h -o " + objdir + "jaclang.h.gch -w -Ofast")
        elif osplatform == "OSX":
            system("g++ -c " + includedir + "jaclang.h -o " + objdir + "jaclang.h.gch -std=gnu++11 -stdlib=libc++ -w -Ofast")

    if threads:
        print("Building jaclang...")
        for thread in threads:
            thread.objlen = len(threads)
            thread.start()
        for thread in threads:
            thread.join()
            if return_fail:
                exit(1)
        columns = int(popen('stty size', 'r').read().split()[1]) - 2
        for i in range(columns + 2):
            print(" ", end='')
        print('\r', end='')

    objfiles = [objdir + file + ".o " for file in files]
    print("Linking object files ... ", end='')
    stdout.flush()
    linker_return_code = call("g++ -W -o jaclang -m64 -std=gnu++11 " + " ".join(objfiles), shell=True)

    if linker_return_code != 0:
        print("FAIL")
        exit(1)
    else:
        print("DONE")


if __name__ == "__main__":
    build()
