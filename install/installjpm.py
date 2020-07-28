import ssl
from getpass import getuser
from os import getcwd, path, mkdir, system
from platform import system as sys
from wget import download
from zipfile import ZipFile

install_folder = "/usr/local/share/"

jpm_version = "1.5.4"


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
    with ZipFile(getcwd() + "/newerjpm.zip", 'r') as zip_ref:
        zip_ref.extractall(getcwd())

    system(f"sudo cp -r jpm-{jpm_version}/jpm-sources {install_folder}")
    system(f"sudo cp jpm-{jpm_version}/jpm /usr/local/bin")

    dirs = [f"{install_folder}jpm-sources/to-install", "/usr/local/share/jaclang-libraries"]

    for Dir in dirs:
        if not path.isdir(Dir):
            mkdir(Dir)
    system(f"rm newerjpm.zip; rm -r jpm-{jpm_version}")

    print("DONE")
    print("Initializing jpm...")
    print()
    system("jpm updatedatabase")


if __name__ == "__main__":
    installjpm_main()
