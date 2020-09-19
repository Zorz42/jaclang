##################
# GLOBAL OPTIONS #
##################

JAC_DIR = /usr/local/Jac

all: jacmake jpm install

$(JAC_DIR):
	@sudo mkdir -p $(JAC_DIR)/Binaries $(JAC_DIR)/Data $(JAC_DIR)/Libraries

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

JACMAKE_VERSION = 1.3.0


jacmake: $(JAC_DIR)
	@echo Installing Jacmake
	@rm -rf jacmake-*
	@curl -L https://github.com/Zorz42/jacmake/archive/v$(JACMAKE_VERSION).tar.gz --output jacmake.tar.gz --silent
	@tar -zxf jacmake.tar.gz
	@sudo rm -rf $(JAC_DIR)/Jacmake $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
	@sudo mv jacmake-$(JACMAKE_VERSION)/Sources $(JAC_DIR)/Jacmake
	@sudo mv jacmake-$(JACMAKE_VERSION)/Data/* $(JAC_DIR)/Data
	@sudo ln -s $(JAC_DIR)/Jacmake/main.py $(JAC_DIR)/Binaries/jacmake
	@sudo ln -s $(JAC_DIR)/Binaries/jacmake /usr/local/bin/jacmake
	@rm -r jacmake-$(JACMAKE_VERSION) jacmake.tar.gz 

#######   
# JPM #
#######

JPM_VERSION = 1.7.14


jpm: $(JAC_DIR)
	@echo Installing Jpm
	@rm -rf jpm-*
	@curl -L https://github.com/Zorz42/jpm/archive/v$(JPM_VERSION).tar.gz --output jpm.tar.gz --silent
	@tar -zxf jpm.tar.gz
	@sudo rm -rf $(JAC_DIR)/Jpm $(JAC_DIR)/Binaries/jpm /usr/local/bin/jpm
	@sudo mv jpm-$(JPM_VERSION)/Sources $(JAC_DIR)/Jpm
	@sudo mv jpm-$(JPM_VERSION)/Data/* $(JAC_DIR)/Data
	@sudo cp jpm-$(JPM_VERSION)/jpm $(JAC_DIR)/Binaries/jpm
	@sudo chmod +x $(JAC_DIR)/Binaries/jpm
	@sudo ln -s $(JAC_DIR)/Binaries/jpm /usr/local/bin/jpm
	@rm -r jpm-$(JPM_VERSION) jpm.tar.gz

###########
# INSTALL #
###########

install: $(JAC_DIR) build
	@echo Installing Jaclang
	@sudo rm -f $(JAC_DIR)/Binaries/jaclang /usr/local/bin/jaclang
	@sudo mv jaclang $(JAC_DIR)/Binaries/jaclang
	@sudo cp Data/* $(JAC_DIR)/Data
	@sudo ln -s $(JAC_DIR)/Binaries/jaclang /usr/local/bin/jaclang
