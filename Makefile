CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -W -Wdeprecated-declarations -Wunused-parameter

UNAME_S := $(shell uname -s)
PKG_CONFIG ?= pkg-config
GLFW_CFLAGS := $(shell $(PKG_CONFIG) --cflags glfw3 2>/dev/null)
GLFW_LIBS := $(shell $(PKG_CONFIG) --libs glfw3 2>/dev/null)

ifeq ($(UNAME_S),Darwin)
INC = -I/opt/homebrew/include
LIB = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq ($(UNAME_S),Linux)
INC = $(GLFW_CFLAGS)

ifneq ($(strip $(GLFW_LIBS)),)
LIB = $(GLFW_LIBS) -lGL -ldl -lpthread -lX11 -lm
else
LIB = -lglfw -lGL -ldl -lm -lpthread -lX11
endif

else
INC =
LIB = -lglfw
endif

BIN = cmake-build-debug
TARGET = $(BIN)/app
SRC = main.c
ENGINE_LIB = $(BIN)/Engine/libengine.a
ENGINE_SRCS = $(shell find Engine -type f \( -name '*.c' -o -name '*.h' \))

.PHONY: all clean run

all: $(TARGET) run

$(ENGINE_LIB): $(ENGINE_SRCS)
	$(MAKE) -C Engine

$(TARGET): $(SRC) $(ENGINE_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $@ $(ENGINE_LIB) $(LIB)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	$(MAKE) -C Engine clean
