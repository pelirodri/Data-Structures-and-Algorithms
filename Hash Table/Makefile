CC := clang
CFLAGS := -std=c2x -Ofast

NAME := hashtables
BUILD_DIR := ./build
LIB := $(BUILD_DIR)/lib/lib$(NAME).a
SRC := $(wildcard ./src/*.c)
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC)))
DEP := $($(filter-out %main.o,$(OBJ)):.o=.d)

$(LIB): $(OBJ)
	mkdir -p $(@D)
	ar rcs $@ $^

include $(DEP)

$(BUILD_DIR)/%.o: ./src/%.c Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -I./include -o $@

.PHONY: install uninstall clean

install:
	cp $(LIB) /usr/local/lib

uninstall:
	$(RM) /usr/local/lib/$(NAME)

clean:
	rm -rfv $(BUILD_DIR)
