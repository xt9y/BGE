CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -W -Wdeprecated-declarations

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

TARGET = app
SRC = src/main.c src/gfx.c src/text.c src/util/math.c src/cam.c

.PHONY: all clean run

all: $(TARGET) run

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $@ $(LIB)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
