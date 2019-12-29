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
	@$(PYTHON) install/main.py dependencies
	@python3 install/installjpm.py
	@python3 install/main.py build
	@python3 install/main.py install

init:
	@$(PYTHON) install/main.py dependencies
jpm:
	@python3 install/installjpm.py
build:
	@python3 install/main.py build
install:
	@python3 install/main.py install

uninstall:
	@sudo rm /usr/local/bin/jaclang

skipjpm:
	@$(PYTHON) install/main.py dependencies
	@python3 install/main.py build
	@python3 install/main.py install

