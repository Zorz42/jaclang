ifeq (,$(which python))
    PYTHON := python
endif
ifeq (,$(which python3))
    PYTHON := python3
endif

cc = g++ $(incldir:%=-I%)
flags = -w -o $@ -m64 -std=gnu++11

objdir  = build
srcdir  = src
incldir = include

all: install

.PHONY: clean install

$(objdir)/%.o: $(srcdir)/%.cpp | $(objdir)
	@echo '[CC] [-FLAGS] $< -> $@'
	@$(cc) $(flags) -c $<

$(objdir):
	@mkdir $@
	@touch $@/*.o

clean:
	@rm -rf $(objdir)

install: install/install.py
	@$(PYTHON) install/install.py dependencies
	@$(PYTHON) install/install.py install

uninstall: install/install.py
	@jaclang uninstall
