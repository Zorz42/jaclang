## Full installation guide

To install you will need:
 * Macos or linux (does not work on windows),
 * 64 bit OS

INSTALLATION PROCESS:

* Download zip file and unzip it or clone it via git,
* open terminal and go to unzipped/cloned folder,
* you will need to install make, python3 and gcc
* execute command:

      make

* jaclang is installed at /usr/local/bin.
* jaclang assets/data are installed in /usr/local/share

To execute separate stages of installation:

    make jacmake
    make jpm
    make build
    make install
    
All together:

    make

If you want to install only jaclang:

    make build
    make install
    
Or all together:

    make onlyjaclang
