from os import getcwd, path, mkdir, system
import ssl
from wget import download
from platform import system as sys
from getpass import getuser


def installjpm_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass

    print()
    print("Downloading jpm ... ", end='', flush=True)
    download("https://github.com/Zorz42/jpm/archive/master.zip", bar=None)
    print("DONE")

    print("Installing jpm ... ", end='', flush=True)
    from zipfile import ZipFile
    with ZipFile(getcwd() + "/jpm-master.zip", 'r') as zip_ref:
        zip_ref.extractall(getcwd())

    dirs = ['to_install', 'librarysources', 'metadatas']
    for Dir in dirs:
        if not path.isdir("jpm-master/" + Dir):
            mkdir('jpm-master/' + Dir)
    if sys() == 'Linux':
        system("sudo cp -r jpm-master/jpm-sources /usr/local/bin")
    elif sys() == 'Darwin':
        system("sudo cp -r jpm-master/jpm-sources /usr/local/bin/jpm-sources")
    else:
        print("Unsupported os!")
    system("sudo cp jpm-master/jpm /usr/local/bin")
    system("sudo chown " + getuser() + " /usr/local/bin/jpm-sources")
    system("sudo chmod +x /usr/local/bin/jpm")


    system("rm jpm-master.zip; rm -r jpm-master")
    print("DONE")
    print("Initializing jpm...")
    print()
    system("sudo jpm repair")
    system("sudo jpm updatedatabase")


if __name__ == "__main__":
    installjpm_main()
