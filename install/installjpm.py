import ssl
from getpass import getuser
from os import getcwd, path, mkdir, system
from platform import system as sys
from wget import download

install_folder = "/usr/local/share/"


def installjpm_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass
    print()
    print("Downloading jpm ... ", end='', flush=True)
    download("https://github.com/Zorz42/jpm/archive/stable.zip", bar=None)
    print("DONE")

    print("Installing jpm ... ", end='', flush=True)
    from zipfile import ZipFile
    with ZipFile(getcwd() + "/jpm-stable.zip", 'r') as zip_ref:
        zip_ref.extractall(getcwd())

    dirs = ['to_install', 'librarysources', 'metadatas']
    for Dir in dirs:
        if not path.isdir("jpm-stable/jpm-sources/" + Dir):
            mkdir('jpm-stable/jpm-sources/' + Dir)
    if path.isdir(install_folder + "/jpm-sources"):
        system("sudo rm -r " + install_folder + "/jpm-sources")
    if sys() == 'Linux':
        system("sudo cp -r jpm-stable/jpm-sources " + install_folder)
    elif sys() == 'Darwin':
        system("sudo cp -r jpm-stable/jpm-sources " + install_folder + "jpm-sources")
    else:
        print("Unsupported os!")
    system("sudo rm /usr/local/bin/jpm")
    system("sudo cp jpm-stable/jpm /usr/local/bin")
    system("sudo chown " + getuser() + " " + install_folder + "jpm-sources")
    system("sudo chmod +x /usr/local/bin/jpm")

    system("rm jpm-stable.zip; rm -r jpm-stable")
    print("DONE")
    print("Initializing jpm...")
    print()
    system("jpm repair")
    system("jpm updatedatabase")


if __name__ == "__main__":
    installjpm_main()
