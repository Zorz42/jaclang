import sys
from os import *

try:
	sys.argv[1]
except:
	print("Input an argument such as install, uninstall, dependencies!")
	exit()

try:
	sys.argv[2]
	print("Only one argument allowed!")
except:
	if(sys.argv[1] == "install"):
		system("g++ src/*.cpp -o jaclang")
		system("sudo cp jaclang /usr/bin/jaclang")
		print("Jaclang installed sucsessfully!")
	elif(sys.argv[1] == "uninstall"):
		system("sudo rm -f /usr/bin/jaclang")
		system("sudo apt remove nasm")
		print("Jaclang uninstalled sucsessfully!")
	elif(sys.argv[1] == "dependencies"):
		print("Checking for dependencies: nasm")
		if(popen("whereis nasm").read() == "nasm:\n"):
			system("sudo apt install nasm")
			print("Installed dependency nasm")
		else:
			print("Nasm is already installed!")
	else:
		print("Invalid argument: " + sys.argv[1])
