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

def check_for_package(name, install_command):
	print(name.upper() + " ... ", end="")
	if popen("which " + name).read() == "":
		print("FAILED")
		if(decision("Do you want me to install " + name + "?")):
			system(install_command)
		else:
			exit(1)
	else:
		print("OK")

if len(sys.argv) == 1:
	print("Input an argument such as install, uninstall, dependencies!")

elif len(sys.argv) == 2:
	if sys.argv[1] == "install":
		system("make jaclang")
		system("sudo mv jaclang /usr/local/bin/jaclang")
		
		if decision("Would you like to clean up object files?"):
			system("make clean")
		
		print("Creating alias...")
		alias_file_name = ""
		if platform.system() == 'Linux':
			alias_file_name = ".bashrc"
		elif platform.system() == 'Darwin':
			alias_file_name = ".bash_profile"
		else:
			print("unsuported os")
			exit(1)

		alias_command = "complete -o default -W \"uninstall version versionid versionstr\" 'jaclang'\n"
		
		system("touch " + path.expanduser("~") + "/" + alias_file_name)
		
		alias_file = open(path.expanduser("~") + "/" + alias_file_name, "r")
		lines = alias_file.readlines()
		if len(lines) != 0:
			lines = lines[-1]
		alias_file.close()

		if lines != alias_command:
			alias_file = open(path.expanduser("~") + "/" + alias_file_name, "a")
			alias_file.write(alias_command)
			alias_file.close()
			print("Created alias!")
		else:
			print("Alias already created!")
		print("Jaclang installed sucsessfully! Type jaclang in terminal for help.")
	
	elif sys.argv[1] == "dependencies":

		if platform.system() == 'Linux':

			print("Checking for dependencies:")
			check_for_package("nasm", "sudo apt install nasm")

		elif platform.system() == 'Darwin':

			print("Checking for dependencies:")
			check_for_package("brew", '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
			check_for_package("nasm", "brew install nasm")

		else:
			print("Unsuported os!")
	else:
		print("Invalid argument: " + sys.argv[1])
else:
	print("Only one argument allowed!")
