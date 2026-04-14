all: deps configure build run

deps:
	git submodule update --init --recursive

configure:
	cmake -S . -B cmake-build-debug

build:
	cmake --build cmake-build-debug --target openglc

run:
	./cmake-build-debug/openglc

add:
	git status
	git add Makefile *.c *.h Engine/*.c Engine/*.h Engine/res/*.h Engine/res/*.png
	git status
