ifeq ($(shell uname),Darwin)
    PYTHON := python
endif
ifeq ($(shell uname),Linux)
    PYTHON := python3
endif

#W = -Wall -Wextra -Wshadow -pedantic
W = -w

objdir  = build
srcdir  = src
incldir = include

cc = g++ $(incldir:%=-I%)
flags = $W -o $@ -m64 -std=gnu++11
objs = astPrinter.o equationParser.o equationGenerator.o error.o functionParser.o generator.o keywords.o lexer.o main.o parser.o preprocessor.o scopeParser.o shortcuts.o functionGenerator.o variableGenerator.o variableParser.o
fullnameobjs = $(patsubst %,$(objdir)/%,$(objs))

all: install

.PHONY: clean install

./jaclang: $(fullnameobjs) | .
	@$(cc) $(flags) $(fullnameobjs)

$(objdir)/%.o: $(srcdir)/%.cpp | $(objdir)
	@echo '[COMPILING] [-o] $< -> $@'
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
