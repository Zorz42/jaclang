import ssl
from getpass import getuser
from os import getcwd, path, mkdir, system
from platform import system as sys
from wget import download

install_folder = "/usr/local/share/"

jpm_version="1.5.3"

def installjpm_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass
    print()
    print("Downloading jpm ... ", end='', flush=True)
    download(f"https://github.com/Zorz42/jpm/archive/v{jpm_version}.zip", "newerjpm.zip", bar=None)
    print("DONE")

    print("Installing jpm ... ", end='', flush=True)
    from zipfile import ZipFile
    with ZipFile(getcwd() + "/newerjpm.zip", 'r') as zip_ref:
        zip_ref.extractall(getcwd())

    
    system(f"sudo cp -r jpm-{jpm_version}/jpm-sources {install_folder}")

    dirs = ['to_install', 'librarysources', 'metadatas']
    for Dir in dirs:
        if not path.isdir(install_folder + "jpm-sources/" + Dir):
            mkdir(install_folder + "jpm-sources/" + Dir)
    system(f"sudo cp jpm-{jpm_version}/jpm /usr/local/bin")
    system("sudo chown " + getuser() + " " + install_folder + "jpm-sources")
    system("sudo chmod +x /usr/local/bin/jpm")

    print("DONE")
    print("Initializing jpm...")
    print()
    system("jpm updatedatabase")
    system(f"rm newerjpm.zip; rm -r jpm-{jpm_version}")


if __name__ == "__main__":
    installjpm_main()
