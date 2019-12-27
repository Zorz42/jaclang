from os import system, path
import platform

alias_file_name = ""
if platform.system() == 'Linux':
	alias_file_name = ".bashrc"
elif platform.system() == 'Darwin':
	alias_file_name = ".bash_profile"
else:
	print("unsupported os")
	exit(1)

alias_command = "complete -o default -W \"uninstall version versionid versionstr\" 'jaclang'\n"
		
system("sudo touch " + path.expanduser("~") + "/" + alias_file_name)
		
alias_file = open(path.expanduser("~") + "/" + alias_file_name, "r")
lines = alias_file.readlines()
if len(lines) != 0:
	lines = lines[-1]
alias_file.close()

if lines != alias_command:
	alias_file = open(path.expanduser("~") + "/" + alias_file_name, "a")
	alias_file.write(alias_command)
	alias_file.close()
