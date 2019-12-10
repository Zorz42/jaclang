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

fullinstall:
	@$(PYTHON) install/main.py all

init:
	@$(PYTHON) install/main.py dependencies
buildc:
	@$(PYTHON) install/main.py build
install:
	@$(PYTHON) install/main.py install

uninstall:
	@jaclang uninstall
