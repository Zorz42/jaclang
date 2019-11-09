import wget, os, ssl
ssl._create_default_https_context = ssl._create_unverified_context
wget.download("https://github.com/Zorz42/jpm/archive/master.zip")
os.system("unzip jpm-master.zip")
os.system("cd jpm-master && python3 install.py install")
os.system("rm jpm-master.zip; rm -r jpm-master")
