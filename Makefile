ifeq (,$(which python))
    PYTHON := python
endif
ifeq (,$(which python3))
    PYTHON := python3
endif

#W = -Wall -Wextra -Wshadow -pedantic
W = -w

objdir  = build
srcdir  = src
incldir = include

cc = g++ $(incldir:%=-I%)
flags = $W -o $@ -m64 -std=gnu++11

all: install

.PHONY: clean install

jaclang: $(objdir)/*.o | .
	@$(cc) $(flags) $(objdir)/*.o

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
