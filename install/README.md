## Full installation guide

To install you will need:
 - Macos or linux (does not work on windows),
 - knowledge of using terminal (infact, why would you even install jaclang if you dont know terminal).

INSTALLATION PROCESS:

* Download zip file and unzip it or clone it via git,
* open terminal and go to unzipped/cloned folder,
* you will need command make and python2 or python3 (if you do not have python, install python3 for faster install time),
* execute command:

      make

(you will need root password, but do not execute sudo make),
* everytime it asks you to install dependency, you must type y or yes else it will stop install script,
* jaclang is installed at /usr/local/bin.

To execute seperate stages of installation:

    make init
    make build
    make install
    make jpm
    
All together:

    make

If you want to install only jaclang:

    make init
    make build
    make install
    
Or all together:

    make onlyjaclang
