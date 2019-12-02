import wget, os, ssl
try:
	ssl._create_default_https_context = ssl._create_unverified_context
except:
	pass

def no_bar(a, b, c):
	pass

print("Downloading jpm")
wget.download("https://github.com/Zorz42/jpm/archive/master.zip", bar=no_bar)
os.system("unzip -q jpm-master.zip")
os.system("cd jpm-master && python3 install.py install")
os.system("rm jpm-master.zip; rm -r jpm-master")
