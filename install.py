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
		if(popen("ls /usr/bin/jaclang").read() == "/usr/bin/jaclang\n"):
			decision = input("Jaclang already installed! Do you want to reinstall / update [y,n]:")
			yesOptions = ["Y", "YES"]
			noOptions  = ["N", "NO"]
			if(decision.upper() in yesOptions):
				pass
			elif(decision.upper() in noOptions):
				exit()
			else:
				print("Wrong answer!")
				exit()
		system("g++ src/*.cpp -o jaclang")
		system("sudo mv jaclang /usr/bin/jaclang")
		print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
	elif(sys.argv[1] == "dependencies"):
		print("Checking for dependencies: nasm")
		if(popen("whereis nasm").read() == "nasm:\n"):
			system("sudo apt install nasm")
			print("Installed dependency nasm")
		else:
			print("Nasm is already installed!")
	else:
		print("Invalid argument: " + sys.argv[1])