import sys
from os import popen, system, path
import platform

python3 = sys.version_info.major == 3

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
		if popen("which jaclang").read() == "/usr/local/bin/jaclang\n":
			try:
				if(python3):
					decision = input("Jaclang already installed! Do you want to reinstall / update [y,n]:")
				else:
					decision = raw_input("Jaclang already installed! Do you want to reinstall / update [y,n]:")
			except:
				pass
			if decision.upper() in yesOptions:
				pass
			elif decision.upper() in noOptions:
				exit()
			else:
				print("Wrong answer!")
				exit()
		system("make jaclang")
		system("sudo mv jaclang /usr/local/bin/jaclang")
		try:
			if(python3):
				decision = input("Would you like to clean up object files[y,n]:")
			else:
				decision = raw_input("Would you like to clean up object files[y,n]:")
		except:
			pass
		if decision.upper() in yesOptions:
			system("make clean")
		elif decision.upper() in noOptions:
			pass
		else:
			print("Wrong answer!")
			exit()
		print("Creating alias...")
		alias_file_name = ""
		if platform.system() == 'Linux':
			alias_file_name = ".bashrc"
		elif platform.system() == 'Darwin':
			alias_file_name = ".bash_profile"
		else:
			print("unsuported os")
			exit()

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
			print("Checking for dependencies: nasm")
			if popen("which nasm").read() == "\n":
				system("sudo apt install nasm")
				print("Installed dependency nasm")
			else:
				print("Nasm is already installed!")
		elif platform.system() == 'Darwin':
			print("Checking for dependencies: brew, nasm")
			if popen("which brew").read() != "/usr/local/bin/brew\n":
				system('/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
			else:
				print("brew is already installed!")
			if popen("ls /usr/local/bin/nasm").read() == '':
				system("brew install nasm")
			else:
				print("nasm is already installed!")
		else:
			print("Unsuported os!")
	else:
		print("Invalid argument: " + sys.argv[1])
