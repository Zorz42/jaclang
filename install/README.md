To install you will need:
 - Macos or linux OS (does not work on windows),
 - knowledge of using terminal (infact, why would you even install jaclang if you dont know terminal).

INSTALLATION PROCESS:

1. Download zip file and unzip it or clone it via git,
2. open terminal and go to unzipped/cloned folder,
3. you will need command make and python2 or python3 (if you do not have python install python3 for faster install time),
4. execute command make (you will need root password, but do not execute sudo make),
5. everytime it asks you to install dependency, you must type y or yes, or it will stop install script
6. jaclang is installed at /usr/local/bin.

EXTRAS:
 - to make full build, type make
 - to only install dependencies, type make dependencies
 - to only build jaclang, type make buildc (jaclang executable will be in jaclang directory)
 - to install jaclang (must be built first), type make install
