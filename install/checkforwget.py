import os

yesOptions = ["Y", "YES"]
noOptions  = ["N", "NO"]

print("Checking for pip3-wget ... ", end='')

try:
	import wget
	print("OK")
except:
	print("FAILED")
	while True:
		decision = input("Do you want me to install pip3-wget?[y,n]")
		if decision.upper() in yesOptions:
			os.system("pip3 install wget")
			break
		elif decision.upper() in noOptions:
			exit(1)
