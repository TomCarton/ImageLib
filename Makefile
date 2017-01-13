# Makefile

LIBRARY := libImaging.a

OBJ_DIR := obj

C_FILES := $(wildcard sources/*.c)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(C_FILES:.c=.o)))

CC=clang
CC_FLAGS = -Wall -O3


all: $(LIBRARY)


$(LIBRARY): $(OBJ_FILES)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: sources/%.c
	@echo Compile "$@"...
	$(CC) $(CC_FLAGS) -c $^ -o $@

$(filter %.o,$(C_FILES)): %.o: %.c %.h
	@echo Compile "$<" to "$@"...
	$(CC) $(CC_FLAGS) -c $< -o $@


$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm $(LIBRARY)


.PHONY: info
info:
	@echo sources: $(C_FILES)
	@echo objects: $(OBJ_FILES)
