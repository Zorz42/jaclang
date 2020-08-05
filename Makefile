all: jacmake jpm build install

clean:
	@rm -rf Objects

build:
	@python3 InstallScripts/build.py
install:
	@sudo python3 InstallScripts/install.py
jpm:
	@sudo python3 InstallScripts/installjpm.py
jacmake:
	@sudo python3 InstallScripts/installjacmake.py

onlyjaclang: build install
