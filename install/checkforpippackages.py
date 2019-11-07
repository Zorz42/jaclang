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
			os.system("sudo pip3 install wget")
			break
		elif decision.upper() in noOptions:
			exit(1)

print("Checking for pip3-httplib2 ... ", end='')

try:
        import httplib2
        print("OK")
except:
        print("FAILED")
        while True:
                decision = input("Do you want me to install pip3-httplib2?[y,n]")
                if decision.upper() in yesOptions:
                        os.system("sudo pip3 install httplib2")
                        break
                elif decision.upper() in noOptions:
                        exit(1)
