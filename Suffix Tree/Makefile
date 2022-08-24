CXX := clang++
CXXFLAGS := -std=c++20 -Ofast

NAME := suffix_tree
BUILD_DIR := ./build
BIN := $(BUILD_DIR)/bin/$(NAME)
SRC := $(wildcard ./src/*.cpp)
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRC)))
DEP := $($(filter-out %main.o,$(OBJ)):.o=.d)

$(BIN): $(OBJ)
	mkdir -p $(@D)
	$(CXX) $^ -o $@

include $(DEP)

$(BUILD_DIR)/%.o: ./src/%.cpp Makefile
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -I./include -o $@

.PHONY: clean

clean:
	rm -rfv $(BUILD_DIR)
