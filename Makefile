make: src/*.cpp
	g++ src/*.cpp
	mv a.out jaclang
	sudo cp jaclang /usr/bin/jaclang
	sudo apt install nasm
	echo Jaclang compiled sucsessfully!
