ifeq (,$(which python))
    PYTHON := python
endif
ifeq (,$(which python3))
    PYTHON := python3
endif

all: fullinstall

.PHONY: clean install

clean:
	@rm -rf build

fullinstall: install/install.py
	@$(PYTHON) install/install.py all

init:
	@$(PYTHON) install/install.py dependencies
buildc:
	@$(PYTHON) install/install.py build
install:
	@$(PYTHON) install/install.py install

uninstall: install/install.py
	@jaclang uninstall
