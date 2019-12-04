import wget, os, ssl

def no_bar(a, b, c):
	pass

def installjpm_main():
	try:
		ssl._create_default_https_context = ssl._create_unverified_context
	except:
		pass

	print()
	print("Downloading jpm ... ", end='', flush=True)
	wget.download("https://github.com/Zorz42/jpm/archive/master.zip", bar=no_bar)
	print("DONE")

	print("Installing jpm ... ", end='', flush=True)
	os.system("unzip -q jpm-master.zip")
	os.system("cd jpm-master && python3 install.py quietinstall")
	os.system("rm jpm-master.zip; rm -r jpm-master")
	print("DONE")

if __name__ == "__main__":
	installjpm_main()