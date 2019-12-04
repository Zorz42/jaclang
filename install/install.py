from __future__ import print_function
from __future__ import division
from builtins import input
from builtins import range
def div(a, b):
	return a / b
try:
	from past.utils import old_div
except:
	old_div = div
from os import popen, system, path, listdir
import platform, subprocess, sys

python3 = sys.version_info.major == 3

if python3:
	from checkforpippackages import *
	from installjpm import *

if python3:
	print("Installing using python3")
else:
	print("Installing using python2")

def decision(question):
    yesOptions = ["Y", "YES"]
    noOptions  = ["N", "NO"]
    while True:
        try:
            if(python3):
                decision = input(question + " [y,n]:")
            else:
                decision = input(question + " [y,n]:")
        except:
            pass
        
        if decision.upper() in yesOptions:
            return True
        elif decision.upper() in noOptions:
            return False

def check_for_package(name, binary, install_command):
	print(name.upper() + ' ... ', end='')
	if popen("which " + binary).read() == "":
		print("FAILED")
		if decision("Do you want me to install " + name + "?"):
			system(install_command)
		else:
			exit(1)
	else:
		print("OK")

def print_progress_bar(compiled, total, length):
	print('[', end='')
	for i in range(int(old_div(length, total) * compiled)):
		print("#", end='')
	for i in range(int(old_div(length, total) * (total - compiled))):
		print(" ", end='')
	print("]\r", end='')
	sys.stdout.flush()

def buildfile(filename, count, objlen):
	columns = int(popen('stty size', 'r').read().split()[1]) - 2
	for i in range(columns + 2):
		print(" ", end='')
	print("\r", end='')
	print("[CC] [-FLAGS] src/" + filename + ".cpp -> build/" + filename + ".o")
	print_progress_bar(count, objlen, columns)
	system("g++ -w -m64 -std=gnu++11 -Iinclude -o build/" + filename + ".o -c src/" + filename + ".cpp")

def build(fail=False):
	if not fail:
		system("sudo echo")
	if not os.path.isdir("build"):	
		system("mkdir build")
	objectfiles = [file.split('.')[0] for file in listdir("src")]
	count = 0
	for file in objectfiles:
		count += 1
		if not os.path.isfile("build/" + file + ".o"):
			buildfile(file, count, len(objectfiles))
		elif os.path.getctime("build/" + file + ".o") < os.path.getctime("src/" + file + ".cpp"):
			buildfile(file, count, len(objectfiles))
	columns = int(popen('stty size', 'r').read().split()[1]) - 2
	for i in range(columns + 2):
		print(" ", end='')
	print('\r', end='')

	
	objnames = ""
	for file in objectfiles:
		objnames += "build/" + file + ".o "
	print("Linking object files ... ", end='')
	sys.stdout.flush()

	linker_return_value = subprocess.Popen("g++ -W -o jaclang -m64 -std=gnu++11 " + objnames, shell=True)
	linker_return_value2 = linker_return_value.communicate()[0]
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

def install():
	system("sudo mv jaclang /usr/local/bin/jaclang")
	
	system("sudo python3 install/makealias.py")
	print()
	print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")

def dependencies():
	system("sudo echo")
	if platform.system() == 'Linux':
		current_package_manager = ''
		package_managers = ['zypper', 'apt', 'yum', 'emerge', 'pacman']
		for package_manager in package_managers:
			if popen("which " + package_manager).read() != "":
				if package_manager == 'apt':
					current_package_manager = 'apt install'
				elif package_manager == 'yum':
					current_package_manager = 'yum install'
				elif package_manager == 'emerge':
					current_package_manager = 'emerge'
				elif package_manager == 'pacman':
					current_package_manager = 'pacman -S'
				elif package_manager == 'zypper':
					current_package_manager = 'zypper in'
				break
		if current_package_manager == '':
			print('Could not find package manager!')
			exit(1)
			
		print("Checking for dependencies:")
		if package_manager == 'zypper':
			check_for_package("g++", "g++", "sudo " + current_package_manager + " gcc-c++")
		else:
			check_for_package("g++", "g++", "sudo " + current_package_manager + " g++")
		packages = [
			("nasm", "nasm"),
			("binutils", "ld"),
			("unzip", "unzip"),
			("python3", "python3"),	
			("python3-pip", "pip3"),
		]
		for package in packages:
			check_for_package(package[0], package[1], "sudo " + current_package_manager + package[0])

	elif platform.system() == 'Darwin':

		print("Checking for dependencies:")
		check_for_package("brew", "brew", '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
		packages = [
			("nasm", "nasm"),
			("gcc", "g++"),
			("binutils", "ld"),
			("unzip", "unzip"),
			("python3", "python3"),
		]
		for package in packages:
			check_for_package(package[0], package[1], "brew install " + package[0])
	else:
		print("Unsuported os!")
		exit(1)

	if python3:
		checkforpippackages_main()
		installjpm_main()
	else:
		system("python3 install/checkforpippackages.py")
		system("python3 install/installjpm.py")
if len(sys.argv) == 1:
	print("Input an argument such as install, uninstall, dependencies!")

elif len(sys.argv) == 2:
	if sys.argv[1] == "dependencies":
		dependencies()
	
	elif sys.argv[1] == "build":
		build()
	
	elif sys.argv[1] == "install":
		install()

	elif sys.argv[1] == "all":
		dependencies()
		build()
		install()
		
	else:
		print("Invalid argument: " + sys.argv[1])
else:
	print("Only one argument allowed!")
