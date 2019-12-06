import os

yesOptions = ["Y", "YES"]
noOptions = ["N", "NO"]


def check_for_pip_package(name):
	print("Checking for pip3-" + name + " ... ", end='', flush=True)

	try:
		__import__(name)
		print("OK")
	except ImportError:
		print("FAILED")
		while True:
			decision = input("Do you want me to install pip3-" + name + "? [y,n]")
			if decision.upper() in yesOptions:
				os.system("sudo pip3 install " + name)
				break
			elif decision.upper() in noOptions:
				exit(1)


def checkforpippackages_main():
	packages = [
		"setuptools",
		"wget",
		"httplib2",
		"zipfile"
	]
	for package in packages:
		check_for_pip_package(package)


if __name__ == "__main__":
	checkforpippackages_main()
