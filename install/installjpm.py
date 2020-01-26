import os
import ssl
import wget
import platform


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
        if not path.isdir("jpm-sources/" + Dir):
            mkdir('jpm-sources/' + Dir)
    if platform.system() == 'Linux':
        system("sudo cp -r jpm-sources/ /usr/local/bin")
    elif platform.system() == 'Darwin':
        system("sudo cp -r jpm-sources/ /usr/local/bin/jpm-sources")
    else:
        print("Unsupported os!")
    system("sudo cp jpm /usr/local/bin")
    system("sudo chown " + getuser() + " /usr/local/bin/jpm-sources")
    system("sudo chmod +x /usr/local/bin/jpm")


    os.system("rm jpm-master.zip; rm -r jpm-master")
    print("DONE")


if __name__ == "__main__":
    installjpm_main()
