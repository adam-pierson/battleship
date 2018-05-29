BIN_DIR := bin
SRC_DIR := src/source
OBJ_DIR := obj
DOC_DIR := docs
INCLUDE := -I./src/header
 
SOURCE := $(wildcard $(SRC_DIR)/*.cpp) 
OBJECT := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCE))
TARGET := $(BIN_DIR)/battleship.exe

$(TARGET) : $(OBJECT) 
	      @echo "building battleship.exe ..."
			@g++ -std=c++11  $^ -o $@ 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
			@g++ -std=c++11 -o $@ -c $^  $(INCLUDE)

clean :
	      @echo "clean up..."
			@rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe
			@rm -rf $(DOC_DIR)/doxygen

doxygen :
	   @doxygen ./Doxyfile

html :
		@firefox $(DOC_DIR)/doxygen/html/battleship__functions_8cpp.html

run :
	   @$(BIN_DIR)/battleship.exe            
