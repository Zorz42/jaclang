import ssl
from getpass import getuser
from os import getcwd, path, mkdir, system
from platform import system as sys
from wget import download

install_folder = "/usr/local/share/"

jacmake_version="1.0.0"

def installjacmake_main():
    try:
        ssl._create_default_https_context = ssl._create_unverified_context
    except AttributeError:
        pass
    print()
    print("Downloading jacmake ... ", end='', flush=True)
    download(f"https://github.com/Zorz42/jacmake/archive/v{jacmake_version}.zip", "newerjacmake.zip", bar=None)
    print("DONE")

    print("Installing jacmake ... ", end='', flush=True)
    from zipfile import ZipFile
    with ZipFile(getcwd() + "/newerjacmake.zip", 'r') as zip_ref:
        zip_ref.extractall(getcwd())


    system(f"sudo cp -r jacmake-{jacmake_version}/jacmake-sources {install_folder}")

    system(f"sudo cp jacmake-{jacmake_version}/jacmake /usr/local/bin")
    system("sudo chown " + getuser() + " " + install_folder + "jacmake-sources")
    system("sudo chmod +x /usr/local/bin/jacmake")

    print("DONE")
    system(f"rm newerjacmake.zip; rm -r jacmake-{jacmake_version}")


if __name__ == "__main__":
    installjacmake_main()

