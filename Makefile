ifeq ($(OS),Windows_NT)
    EXE = cmake-build-debug/Debug/openglc.exe
else
    EXE = cmake-build-debug/openglc
endif

all: deps configure build run

deps:
	git submodule update --init --recursive

configure:
	cmake -S . -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target openglc

run:
	$(EXE)

clean:
	rm -rf cmake-build-debug compile_commands.json

add:
	git status
	git add Makefile README.md *.c *.h Engine/*.c Engine/util/*.c Engine/util/*.h Engine/*.h Engine/res/*.h Engine/res/*.png
	git status
