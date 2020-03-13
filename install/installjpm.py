import ssl
from getpass import getuser
from os import getcwd, path, mkdir, system
from platform import system as sys
from wget import download

home_folder = path.expanduser("~")
install_folder = home_folder + "/.local/share/"

def installjpm_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass
    if not path.isdir(home_folder + "/.local/"):
        mkdir(home_folder + "/.local")
    if not path.isdir(install_folder):
        mkdir(install_folder)
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
        if not path.isdir("jpm-master/jpm-sources/" + Dir):
            mkdir('jpm-master/jpm-sources/' + Dir)
    if sys() == 'Linux':
        system("sudo cp -r jpm-master/jpm-sources " + install_folder)
    elif sys() == 'Darwin':
        system("sudo cp -r jpm-master/jpm-sources " + install_folder + "jpm-sources")
    else:
        print("Unsupported os!")
    system("sudo cp jpm-master/jpm /usr/local/bin")
    system("sudo chown " + getuser() + " " + install_folder + "jpm-sources")
    system("sudo chmod +x /usr/local/bin/jpm")

    system("rm jpm-master.zip; rm -r jpm-master")
    print("DONE")
    print("Initializing jpm...")
    print()
    system("sudo jpm repair")
    system("sudo jpm updatedatabase")


if __name__ == "__main__":
    installjpm_main()
