CC := gcc
CFLAGS := -std=c89 -pedantic -Wall -Wextra
LDFLAGS := -lSDL2

BUILD := ../build
TARGET := $(BUILD)/run
OBJ := $(BUILD)/main.o

.PHONY: all clean

all: $(TARGET)
	@echo "Running..."
	@$(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking..."
	@$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ): main.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -vf $(OBJ) $(TARGET)
