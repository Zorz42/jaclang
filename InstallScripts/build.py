from os import system, path, listdir, mkdir
from platform import system as sys
from shutil import get_terminal_size
from threading import Thread

obj_dir = "Objects/"
src_dir = "Sources/"
include_dir = "Headers/"
stdlib = "gnu++17"
optimisation = "2"
warnings = "-Wall -Wshadow -Wextra -Wno-deprecated"

if sys() != "Darwin" and sys() != "Linux":
    print("Unsupported platform!")
    exit(1)


def getColumns():
    return get_terminal_size().columns


class BuildThread(Thread):
    compiled_count = 0

    def __init__(self, filename):
        Thread.__init__(self)
        self.threadID = 0
        self.name = filename

    def run(self):
        compile_command = ""
        if sys() == "Linux":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{obj_dir} " \
                              f"-o {obj_dir}{self.name}.o -c {src_dir}{self.name}.cpp"
        elif sys() == "Darwin":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{include_dir}" \
                              f" -o {obj_dir}{self.name}.o -c {src_dir}{self.name}.cpp " \
                              f"-include-pch {obj_dir}jaclang.h.gch -D IGNORE_MAIN_INCLUDE"
        current_thread = system(compile_command)
        if current_thread != 0:
            exit(1)
        else:
            BuildThread.compiled_count += 1

        columns = get_terminal_size().columns - 2
        print(f"[{'#' * int(columns / self.threadID * BuildThread.compiled_count)}"
              f"{' ' * int(columns / self.threadID * (self.threadID - BuildThread.compiled_count))}]\r", end='')


def build():
    if not path.isdir(obj_dir):
        mkdir(obj_dir)

    files = [file.split('.')[0] for file in listdir(src_dir) if not path.isdir(src_dir + file)]

    for dir_ in listdir(src_dir):
        if path.isdir(src_dir + dir_):
            if not path.isdir(obj_dir + dir_):
                mkdir(obj_dir + dir_)
            files += [path.join(dir_, file.split('.')[0]) for file in listdir(src_dir + dir_)]

    threads, build_header = [], False

    for file in listdir(include_dir):
        if not path.isfile(f"{obj_dir}jaclang.h.gch") or path.getctime(f"{obj_dir}jaclang.h.gch") < \
                path.getctime(include_dir + file):
            build_header = True

    for file in files:
        if build_header or not path.isfile(f"{obj_dir}{file}.o") or \
                path.getctime(f"{obj_dir}{file}.o") < path.getctime(f"{src_dir}{file}.cpp"):
            threads.append(BuildThread(file))

    if build_header:
        print("Building jaclang header.")
        system(f"g++ {warnings} -pipe -m64 -c -O{optimisation} -g {include_dir}jaclang.h -o {obj_dir}jaclang.h.gch -std={stdlib}")

    if threads:
        print("Building jaclang source files.")
        for thread in threads:
            thread.threadID = len(threads)
            thread.start()
        for thread in threads:
            thread.join()
        print(f"{' ' * getColumns()}\r", end='')

    if threads:
        print("Linking object files.")

    if system(f"g++ -m64 -std={stdlib} -o jaclang " + " ".join([f"{obj_dir}{file}.o" for file in files])):
        exit(1)


if __name__ == "__main__":
    build()
