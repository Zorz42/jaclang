install: install.py
	python3 install.py install
	python3 install.py dependencies

uninstall: install.py
	python3 install.py uninstall
