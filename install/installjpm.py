import os
import ssl
import wget


def installjpm_main():
    ssl._create_default_https_context = ssl._create_unverified_context

    print()
    print("Downloading jpm ... ", end='', flush=True)
    wget.download("https://github.com/Zorz42/jpm/archive/master.zip", bar=None)
    print("DONE")

    print("Installing jpm ... ", end='', flush=True)
    os.system("unzip -q jpm-master.zip")
    os.system("cd jpm-master && python3 install.py quietinstall")
    os.system("rm jpm-master.zip; rm -r jpm-master")
    print("DONE")


if __name__ == "__main__":
    installjpm_main()
