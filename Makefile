TARGET=ucm

# gcc or clang
CC=gcc

# out
BUILD=build
OUT=$(BUILD)/out
$(shell if [ ! -e $(OUT) ]; then mkdir -p $(OUT); fi)

# source file
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o,$(SRC))

# arguments
CFLAGES = -c -Wall -g
CFLAGES += -Iinclude

$(TARGET): $(patsubst %.o, $(BUILD)/%.o,$(OBJ))
	$(CC) $^ -o $(OUT)/$@

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGES) $< -o $@

# clean
.PHONY:
	clean

clean:
	rm -rf $(BUILD)