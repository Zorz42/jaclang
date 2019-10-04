import sys
from os import popen, system
import platform

try:
	sys.argv[1]
except:
	print("Input an argument such as install, uninstall, dependencies!")
	exit()

try:
	sys.argv[2]
	print("Only one argument allowed!")
except:
	if sys.argv[1] == "install":
		yesOptions = ["Y", "YES"]
		noOptions  = ["N", "NO"]
		if popen("whereis jaclang").read() == "jaclang: /usr/local/bin/jaclang\n":
			decision = raw_input("Jaclang already installed! Do you want to reinstall / update [y,n]:")
			if decision.upper() in yesOptions:
				pass
			elif decision.upper() in noOptions:
				exit()
			else:
				print("Wrong answer!")
				exit()
		system("make build/*.o")
		system("make jaclang")
		system("sudo mv jaclang /usr/local/bin/jaclang")
		print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
		decision = raw_input("Would you like to clean up object files[y,n]:")
		if decision.upper() in yesOptions:
			system("make clean")
		elif decision.upper() in noOptions:
			exit()
		else:
			print("Wrong answer!")
			exit()
	elif sys.argv[1] == "dependencies":
		if platform.system() == 'Linux':
			print("Checking for dependencies: nasm")
			if popen("whereis nasm").read() == "nasm:\n":
				system("sudo apt install nasm")
				print("Installed dependency nasm")
			else:
				print("Nasm is already installed!")
		elif platform.system() == 'Darwin':
			print("Checking for dependencies: brew, nasm")
			if popen("brew").read() == '':
				system('/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
			else:
				print("brew is already installed!")
			if popen("ls /usr/local/bin/nasm").read() == '':
				system("brew install nasm")
			else:
				print("nasm is already installed")
		else:
			print("unsuported os")
	else:
		print("Invalid argument: " + sys.argv[1])
