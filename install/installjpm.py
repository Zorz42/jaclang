import os
import ssl
import wget
import platform
from getpass import getuser


def installjpm_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass

    print()
    print("Downloading jpm ... ", end='', flush=True)
    wget.download("https://github.com/Zorz42/jpm/archive/master.zip", bar=None)
    print("DONE")

    print("Installing jpm ... ", end='', flush=True)
    os.system("unzip -q jpm-master.zip")


    dirs = ['to_install', 'librarysources', 'metadatas']
    for Dir in dirs:
        if not os.path.isdir("jpm-master/" + Dir):
            os.mkdir('jpm-master/' + Dir)
    if platform.system() == 'Linux':
        os.system("sudo cp -r jpm-master/ /usr/local/bin")
    elif platform.system() == 'Darwin':
        os.system("sudo cp -r jpm-master/ /usr/local/bin/jpm-master")
    else:
        print("Unsupported os!")
    os.system("sudo cp jpm /usr/local/bin")
    os.system("sudo chown " + getuser() + " /usr/local/bin/jpm-master")
    os.system("sudo chmod +x /usr/local/bin/jpm")


    os.system("rm jpm-master.zip; rm -r jpm-master")
    print("DONE")


if __name__ == "__main__":
    installjpm_main()
