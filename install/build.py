from os import system, path, listdir, popen
from platform import system as sys
from subprocess import call
from sys import stdout
from threading import Thread

obj_dir = "obj/"
src_dir = "src/"
include_dir = "include/"
stdlib = "gnu++17"
optimisation = "fast"
warnings = "-Wall -Wshadow -Wextra -Wno-deprecated -Wno-macro-redefined"

compiled_count = 0
return_fail = False
os_platform: str

if sys() == "Linux":
    os_platform = "linux"
elif sys() == "Darwin":
    os_platform = "OSX"
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
        self.threadID = 0
        self.name = filename

    def run(self):
        global compiled_count
        compile_command = ""
        if os_platform == "linux":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{include_dir} -I{obj_dir} " \
                              f"-o {obj_dir}{self.name}.o -c {src_dir}{self.name}.cpp -D IGNORE_MAIN_INCLUDE"
        elif os_platform == "OSX":
            compile_command = f"g++ {warnings} -pipe -m64 -O{optimisation} -std={stdlib} -I{include_dir}" \
                              f" -o {obj_dir}{self.name}.o -c {src_dir}{self.name}.cpp "\
                              f"-include-pch {obj_dir}jaclang.h.gch -D IGNORE_MAIN_INCLUDE"
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
        print(f"[{success}] {src_dir}{self.name}.cpp -> {obj_dir}{self.name}.o")
        print_progress_bar(compiled_count, self.threadID, columns - 2)


def build():
    print()

    if not path.isdir(obj_dir):
        system(f"mkdir {obj_dir}")
    files = [file.split('.')[0] for file in listdir(src_dir) if not path.isdir(f"{src_dir}{file}")]
    src_files_dirs = [Dir for Dir in listdir(src_dir) if len(Dir.split('.')) == 1]

    for Dir in src_files_dirs:
        if not path.isdir(obj_dir + Dir):
            system(f"mkdir {obj_dir}{Dir}")
        files += [Dir + "/" + str(file.split('.')[0]) for file in listdir(src_dir + Dir) if len(file.split('.')) == 2]

    threads = []
    build_header = False

    for file in listdir(include_dir):
        if not path.isfile(f"{obj_dir}jaclang.h.gch") or path.getctime(f"{obj_dir}jaclang.h.gch") < \
                path.getctime(include_dir + file):
            build_header = True

    for file in files:
        if build_header or not path.isfile(f"{obj_dir}{file}.o") or \
                path.getctime(f"{obj_dir}{file}.o") < path.getctime(f"{src_dir}{file}.cpp"):
            threads.append(BuildThread(file))

    if build_header:
        print("Building jaclang header...")
        if os_platform == "linux":
            system(f"g++ {warnings} -c -O{optimisation} {include_dir}jaclang.h -o {obj_dir}jaclang.h.gch -std={stdlib}")
        elif os_platform == "OSX":
            system(f"g++ {warnings} -c -O{optimisation} {include_dir}jaclang.h -o {obj_dir}jaclang.h.gch -std={stdlib}")

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

    objfiles = [obj_dir + file + ".o" for file in files]
    print("Linking object files... ")
    linker_return_code = 0
    if os_platform == "linux":
        linker_return_code = call(f"g++ -m64 -std={stdlib} -o jaclang " + " ".join(objfiles), shell=True)
    elif os_platform == "OSX":
        linker_return_code = call(f"g++ -m64 -std={stdlib} -o jaclang " + " ".join(objfiles), shell=True)

    if linker_return_code != 0:
        print("Linking failed!")
        exit(1)


if __name__ == "__main__":
    build()
