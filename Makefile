all: default run

default:
	g++ src/* -g -o main.exe -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
run:
	./main.exe