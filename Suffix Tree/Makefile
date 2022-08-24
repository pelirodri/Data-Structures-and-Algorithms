CXX := clang++
CXXFLAGS := -std=c++20 -Ofast

NAME := suffixtree
BUILD_DIR := ./build
LIB := $(BUILD_DIR)/lib/lib$(NAME).a
SRC := $(wildcard ./src/*.cpp)
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRC)))
DEP := $($(OBJ):.o=.d)

$(LIB): $(OBJ)
	mkdir -p $(@D)
	ar rcs $@ $^

include $(DEP)

$(BUILD_DIR)/%.o: ./src/%.cpp Makefile
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -I./include -I./src -o $@

.PHONY: install uninstall clean

install:
	cp $(LIB) /usr/local/lib

uninstall:
	$(RM) /usr/local/lib/lib$(NAME).a

clean:
	rm -rfv $(BUILD_DIR)
