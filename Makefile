W = -Wall -Wextra -Wshadow -pedantic

objdir  = build
srcdir  = src
incldir = include

cc = g++ $(incldir:%=-I%)
flags = $W -o $@ -m64 -std=gnu++11
objs = astPrinter.o equation.o equationGenerator.o error.o functionParser.o generator.o keywords.o lexer.o main.o parser.o preprocessor.o scopes.o shortcuts.o systemFunctionCall.o variableDeclaration.o variableDeclarationParser.o
fullnameobjs = $(patsubst %,$(objdir)/%,$(objs))

all: install

.PHONY: clean install

./jaclang: $(fullnameobjs) | .
	$(cc) $(flags) $(fullnameobjs)

$(objdir)/%.o: $(srcdir)/%.cpp | $(objdir)
	$(cc) $(flags) -c $<

$(objdir):
	mkdir $@

clean:
	rm -rf $(objdir)

install: install.py
	python3 install.py install
	python3 install.py dependencies

uninstall: install.py
	jaclang uninstall
