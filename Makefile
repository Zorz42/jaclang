ifeq (,$(which python))
    PYTHON := python
endif
ifeq (,$(which python3))
    PYTHON := python3
endif

all: fullinstall

.PHONY: clean install

clean:
	@rm -rf obj

fullinstall:
	@$(PYTHON) install/dependencies.py
	@python3 install/build.py
	@python3 install/install.py
	@sudo python3 install/installjpm.py

init:
	@$(PYTHON) install/dependencies.py
jpm:
	@sudo python3 install/installjpm.py
build:
	@python3 install/build.py
install:
	@python3 install/install.py

uninstall:
	@sudo rm /usr/local/bin/jaclang

onlyjaclang:
	@$(PYTHON) install/dependencies.py
	@python3 install/build.py
	@python3 install/install.py
