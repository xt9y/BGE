CC = gcc
CFLAGS = -std=c99 -Wall -Wextra
INC = -I/opt/homebrew/include
LIB = -L/opt/homebrew/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = app
SRC = src/main.c src/gfx.c src/text.c

all: $(TARGET) run

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(INC) $(LIB)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

