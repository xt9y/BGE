ifeq ($(OS),Windows_NT)
    EXE = cmake-build-debug/Debug/cpuc.exe
else
    EXE = cmake-build-debug/cpuc
endif

all: deps configure build run

deps:
	git submodule update --init --recursive

configure:
	cmake -S . -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target cpuc

run:
	$(EXE)

clean:
	rm -rf cmake-build-debug compile_commands.json

add:
	git status
	git add CMakeLists.txt Makefile README.md *.c *.h Engine/CMakeLists.txt Engine/*.c Engine/util/*.c Engine/util/*.h Engine/*.h 
	git status
