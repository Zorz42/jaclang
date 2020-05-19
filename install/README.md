## Full installation guide

To install you will need:
 - Macos or linux (does not work on windows),
 - 64 bit computer

INSTALLATION PROCESS:

* Download zip file and unzip it or clone it via git,
* open terminal and go to unzipped/cloned folder,
* you will need command make and python2 or python3 (if you do not have python, install python3 (recommended)),
* execute command:

      make

(you will need root password, but do not execute sudo make),
* everytime it asks you to install dependency, you must type y or yes else it will stop install script,
* jaclang is installed at /usr/local/bin.
* jaclang assets/data are installed in /usr/local/share

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

##Explanation of stages

    make init

Init checks for dependencies and installs missing ones.

    make build

Build builds jaclang from source code into executable which is placed into jaclang folder

    make install
    
Install places built jaclang into /usr/local/bin and jaclang-data into /usr/local/share
