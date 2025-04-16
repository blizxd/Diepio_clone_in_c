# Compiler and flags
DEBUG ?= 1

CC := gcc
CFLAGS := -g -Wall -Wextra -std=c99 -I./include -I./SDL2/include 
LDFLAGS := -L./SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

ifeq ($(DEBUG), 0)
CFLAGS += -O3
else
CFLAGS += -g -Ddebug
endif

# Directories
SRC_DIR := src
OBJ_DIR := build/obj
BIN_DIR := build/bin

SDL2_BIN_DIR := SDL2/bin

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.d,$(SRC_FILES))

# Executable name
EXECUTABLE := $(BIN_DIR)/main.exe

# Default target
all: directories assets $(EXECUTABLE)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BIN_DIR)/savegames

assets:
	cp -rn assets $(BIN_DIR)/
	cp -n $(SDL2_BIN_DIR)/*.dll $(BIN_DIR)/

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c makefile
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP

# Link object files to create executable
$(EXECUTABLE): $(OBJ_FILES) 
	$(CC) $^ -o $@ $(LDFLAGS) 

# Run the game
run: all
	cd $(BIN_DIR) && ./main.exe

test:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
	@echo $(DEP_FILES)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*.exe 

# Remove build directory
distclean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Include dependencies 
-include $(DEP_FILES)

# Prevent make from treating these targets as files
.PHONY: all directories run clean distclean help test assets