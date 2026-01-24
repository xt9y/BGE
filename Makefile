CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -W -Wdeprecated-declarations
INC = -I/opt/homebrew/include
LIB = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = app
SRC = src/main.c src/gfx.c src/text.c src/util/math.c src/cam.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC) run
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $@ $(LIB)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)