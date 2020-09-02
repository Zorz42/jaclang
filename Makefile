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


all: jacmake jpm build install

_set:
	export PATH=${PATH}:/usr/local/Jac/Binaries

build: $(OBJ_DIR)/$(OBJ_FILES)
	@echo Linking object files
	@g++ -m64 -o jaclang $(OBJ_FILES)

install:
	@sudo env "PATH=${PATH}" python3 InstallScripts/install.py

jpm:
	@sudo python3 InstallScripts/installjpm.py

jacmake:
	@sudo python3 InstallScripts/installjacmake.py

onlyjaclang: build install


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
