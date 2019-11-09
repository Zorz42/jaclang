import sys
from os import popen, system, path
import platform

python3 = sys.version_info.major == 3

def decision(question):
	yesOptions = ["Y", "YES"]
	noOptions  = ["N", "NO"]
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
	else:
		print("Wrong answer!")
		exit(1)

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

if len(sys.argv) == 1:
	print("Input an argument such as install, uninstall, dependencies!")

elif len(sys.argv) == 2:
	if sys.argv[1] == "install":
		system("sudo echo")
		system("make jaclang")
		system("sudo mv jaclang /usr/local/bin/jaclang")
		
		if decision("Would you like to clean up object files?"):
			system("make clean")
		system("sudo python3 install/makealias.py")
		print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
	
	elif sys.argv[1] == "dependencies":
		system("sudo echo")
		if platform.system() == 'Linux':
			current_package_manager = ''
			package_managers = ['apt', 'yum', 'emerge', 'pacman', 'zypper']
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
			check_for_package("jpm", "jpm", "python3 install/installjpm.py")

		elif platform.system() == 'Darwin':

			print("Checking for dependencies:")
			check_for_package("brew", "brew", '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
			check_for_package("nasm", "nasm", "brew install nasm")
			check_for_package("gcc", "gcc", "brew install gcc")
			check_for_package("binutils", "ld", "brew install binutils")
			check_for_package("unzip", "unzip", "brew install unzip")
			check_for_package("python3", "python3", "brew install python3")
			system("python3 install/checkforpippackages.py")
			check_for_package("jpm", "jpm", "python3 install/installjpm.py")
		else:
			print("Unsuported os!")
	else:
		print("Invalid argument: " + sys.argv[1])
else:
	print("Only one argument allowed!")
