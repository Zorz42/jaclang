##################
# GLOBAL OPTIONS #
##################

JAC_DIR = /usr/local/Jac

all: jacmake jpm build install
onlyjaclang: build install

$(JAC_DIR):
	mkdir -p $(JAC_DIR)/Binaries $(JAC_DIR)/Data $(JAC_DIR)/Libraries

#########
# BUILD #
#########

SRC_DIR = Sources
OBJ_DIR = Objects
HEADER_DIR = Headers

SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))


GCC_FLAGS = -Wall -Wshadow -Wextra -Wno-deprecated -m64 -std=gnu++17 -Ofast
GCC_SOURCE_FLAGS = $(GCC_FLAGS)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    GCC_SOURCE_FLAGS += -I $(OBJ_DIR)
endif
ifeq ($(UNAME_S), Darwin)
    GCC_SOURCE_FLAGS += -include-pch $(OBJ_DIR)/jaclang.h.gch -D IGNORE_MAIN_INCLUDE
endif

build: $(OBJ_DIR)/$(OBJ_FILES)
	@echo Linking object files
	@g++ -m64 -o jaclang $(OBJ_FILES)

clean:
	@rm -r Objects

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)/jaclang.h.gch
	@echo Compiling $<
	@mkdir -p $(@D)
	@g++ $(GCC_SOURCE_FLAGS) -o $@ -c $<

$(OBJ_DIR)/jaclang.h.gch: $(HEADER_DIR)/*.h $(SRC_DIR)/*.cpp
	@echo Compiling jaclang header
	@g++ $(GCC_FLAGS) -c $(HEADER_DIR)/jaclang.h -o $(OBJ_DIR)/jaclang.h.gch

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

###########
# JACMAKE #
###########

JACMAKE_VERSION = 1.2.9


jacmake: $(JAC_DIR)
	@echo Installing Jacmake
	@rm -rf jacmake-*
	@curl -L https://github.com/Zorz42/jacmake/archive/v$(JACMAKE_VERSION).tar.gz --output jacmake.tar.gz --silent
	@tar -zxf jacmake.tar.gz
	@rm -rf $(JAC_DIR)/Jacmake $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
	@mv jacmake-$(JACMAKE_VERSION)/Sources $(JAC_DIR)/Jacmake
	@mv jacmake-$(JACMAKE_VERSION)/Data/* $(JAC_DIR)/Data
	@ln -s $(JAC_DIR)/Jacmake/main.py $(JAC_DIR)/Binaries/jacmake
	@ln -s $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
	@rm -r jacmake-$(JACMAKE_VERSION) jacmake.tar.gz 

#######   
# JPM #
#######

JACMAKE_VERSION = 1.2.9


jacmake: $(JAC_DIR)
        @echo Installing Jacmake
        @rm -rf jacmake-*
        @curl -L https://github.com/Zorz42/jacmake/archive/v$(JACMAKE_VERSION).tar.gz --output jacmake.tar.gz --silent
        @tar -zxf jacmake.tar.gz
        @rm -rf $(JAC_DIR)/Jacmake $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
        @mv jacmake-$(JACMAKE_VERSION)/Sources $(JAC_DIR)/Jacmake
        @mv jacmake-$(JACMAKE_VERSION)/Data/* $(JAC_DIR)/Data
        @ln -s $(JAC_DIR)/Jacmake/main.py $(JAC_DIR)/Binaries/jacmake
        @ln -s $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
        @rm -r jacmake-$(JACMAKE_VERSION) jacmake.tar.gz

###########
# INSTALL #
###########

install:
	@sudo env "PATH=${PATH}" python3 InstallScripts/install.py
