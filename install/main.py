import sys
from build import *
from dependencies import *
from install import install

if len(sys.argv) == 1:
    print("Input an argument such as install, uninstall, dependencies!")

elif len(sys.argv) == 2:
    if sys.argv[1] == "dependencies":
        dependencies()

    elif sys.argv[1] == "build":
        build()

    elif sys.argv[1] == "install":
        install()
    elif sys.argv[1] == "buildinstall":
        build()
        install()
    else:
        print("Invalid argument: " + sys.argv[1])
else:
    print("Only one argument allowed!")
