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

fullinstall: init jacmake jpm build install

init:
	@sudo $(PYTHON) install/dependencies.py
build:
	@python3 install/build.py
install:
	@sudo python3 install/install.py
jpm:
	@sudo python3 install/installjpm.py
jacmake:
	@sudo python3 install/installjacmake.py

onlyjaclang: init build install
