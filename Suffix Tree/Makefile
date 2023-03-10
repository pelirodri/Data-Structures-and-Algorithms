CXX := clang++
CXXFLAGS := -I./include -I./src -std=c++20 -Ofast

NAME := suffixtree
BUILD := ./build
LIB := $(BUILD)/lib/lib$(NAME).a
SRC := $(wildcard ./src/*.cpp)
OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(notdir $(SRC)))
DEP := $($(OBJ):.o=.d)

$(LIB): $(OBJ)
	mkdir -p $(@D)
	ar rcs $@ $^

include $(DEP)

$(BUILD)/%.o: ./src/%.cpp $(wildcard ./src/*.hpp) $(wildcard ./include/*.hpp) Makefile
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

.PHONY: install uninstall clean

install:
	cp $(LIB) /usr/local/lib

uninstall:
	$(RM) /usr/local/lib/lib$(NAME).a

clean:
	rm -rfv $(BUILD_DIR)
