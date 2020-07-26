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

fullinstall: init jpm jacmake build install

init:
	@$(PYTHON) install/dependencies.py
build:
	@python3 install/build.py
install:
	@python3 install/install.py
jpm:
	@sudo python3 install/installjpm.py
jacmake:
	@sudo python3 install/installjacmake.py

onlyjaclang: init build install
