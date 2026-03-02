CC 		:= cc
C_INCLUDE 	:= -Iinclude
C_FLAGS 	:=  -lraylib -lm -g -Wall -Wextra

BUILD_DIR 	:= bin
BIN_PATH 	:= $(BUILD_DIR)/main

SRC_DIR 	:= src
SRC_FILES 	:= $(shell find $(SRC_DIR) -type f -name '*.c')

all: mkdir_build compile

compile:
	echo $(SRC_FILES)
	$(CC) -o $(BIN_PATH) $(SRC_FILES) $(C_INCLUDE) $(C_FLAGS)

mkdir_build:
	mkdir -p $(BUILD_DIR)

run: all
	$(BIN_PATH)
