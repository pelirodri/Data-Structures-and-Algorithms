CC := clang
CFLAGS := -I./include -std=c2x -Ofast

NAME := hashtable
BUILD := ./build
LIB := $(BUILD)/lib/lib$(NAME).a
SRC := $(wildcard ./src/*.c)
OBJ := $(patsubst %.c,$(BUILD)/%.o,$(notdir $(SRC)))
DEP := $($(OBJ):.o=.d)

$(LIB): $(OBJ)
	mkdir -p $(@D)
	ar rcs $@ $^

include $(DEP)

$(BUILD)/%.o: ./src/%.c $(wildcard ./include/*.h) Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

.PHONY: install uninstall clean

install:
	cp $(LIB) /usr/local/lib

uninstall:
	$(RM) /usr/local/lib/lib$(NAME).a

clean:
	rm -rfv $(BUILD)
