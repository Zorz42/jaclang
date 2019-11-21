import sys
from os import popen, system, path, listdir
import platform, subprocess

python3 = sys.version_info.major == 3

def decision(question):
    yesOptions = ["Y", "YES"]
    noOptions  = ["N", "NO"]
    while True:
        try:
            if(python3):
                decision = input(question + " [y,n]:")
            else:
                decision = raw_input(question + " [y,n]:")
        except:
            pass
        
        if decision.upper() in yesOptions:
            return True
        elif decision.upper() in noOptions:
            return False

def check_for_package(name, binary, install_command):
	prefix = name.upper() + " ... "
	if popen("which " + binary).read() == "":
		print(prefix + "FAILED")
		if decision("Do you want me to install " + name + "?"):
			system(install_command)
		else:
			exit(1)
	else:
		print(prefix + "OK")

def install(fail=False):
	if not fail:
		system("sudo echo")
	objectfiles = [file.split('.')[0] for file in listdir("src")]
	system("make build --no-print-directory")
	for file in objectfiles:
		system("make build/" + file + ".o --no-print-directory")
	
	objnames = ""
	for file in objectfiles:
		objnames += "build/" + file + ".o "
	print("linking object files...")

	linker_return_value = subprocess.Popen("g++ -W -o jaclang -m64 -std=gnu++11 " + objnames, shell=True)
	linker_return_value2 = linker_return_value.communicate()[0]
	linker_return_code = linker_return_value.returncode
	if linker_return_code != 0:
		if not fail:
			print("Linker failed.")
			print("Recompiling the whole program from scratch.")
			system("make clean")
			install(True)
		else:
			print("Program failed twice. There is a syntax error. Aborting...")
			exit(1)

	if not fail:
		system("sudo mv jaclang /usr/local/bin/jaclang")
		
		system("sudo python3 install/makealias.py")
		print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")

if len(sys.argv) == 1:
	print("Input an argument such as install, uninstall, dependencies!")

elif len(sys.argv) == 2:
	if sys.argv[1] == "install":
		install()
	
	elif sys.argv[1] == "dependencies":
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
			check_for_package("nasm", "nasm", "sudo " + current_package_manager + " nasm")
			if package_manager == 'zypper':
				check_for_package("g++", "g++", "sudo " + current_package_manager + " gcc-c++")
			else:
				check_for_package("g++", "g++", "sudo " + current_package_manager + " g++")
			check_for_package("binutils", "ld", "sudo " + current_package_manager + " binutils")
			check_for_package("unzip", "unzip", "sudo " + current_package_manager + " unzip")
			check_for_package("python3", "python3", "sudo " + current_package_manager + " python3")
			check_for_package("python3-pip", "pip3", "sudo " + current_package_manager + " python3-pip")
			from checkforpippackages import *
			system("python3 install/installjpm.py")

		elif platform.system() == 'Darwin':

			print("Checking for dependencies:")
			check_for_package("brew", "brew", '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
			check_for_package("nasm", "nasm", "brew install nasm")
			check_for_package("gcc", "gcc", "brew install gcc")
			check_for_package("binutils", "ld", "brew install binutils")
			check_for_package("unzip", "unzip", "brew install unzip")
			check_for_package("python3", "python3", "brew install python3")
			system("python3 install/checkforpippackages.py")
			system("python3 install/installjpm.py")
		else:
			print("Unsuported os!")
	else:
		print("Invalid argument: " + sys.argv[1])
else:
	print("Only one argument allowed!")
