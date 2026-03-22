CC ?= gcc
CFLAGS ?= -std=c99 -g -Wall -Wextra -W -Wdeprecated-declarations -Wunused-parameter

UNAME_S := $(shell uname -s)
PKG_CONFIG ?= pkg-config
GLFW_CFLAGS := $(shell $(PKG_CONFIG) --cflags glfw3 2>/dev/null)

INC = -I. -I.. $(GLFW_CFLAGS)

BIN = cmake-build-debug
SRC = App.c cam.c gfx.c prim.c text.c util/math.c
OBJ = $(SRC:%.c=../$(BIN)/Engine/%.o)
TARGET = ../$(BIN)/Engine/libengine.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

../$(BIN)/Engine/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
